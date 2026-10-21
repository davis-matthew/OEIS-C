/*
 * A087520  Smallest prime p such that the prime signature of p-1 has not
 *          occurred earlier.
 * https://oeis.org/A087520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 102, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087520 – smallest prime p such that the prime signature of p‑1 has not occurred earlier
   Algorithm: iterate over primes, compute the multiset of exponent values in the factorisation
   of p‑1, keep a list of seen multisets, and record the next prime whose signature is new.
   The function works for any build (uint8_t … __int128, arbitrary‑precision) and
   supports out‑of‑order requests. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- static state ----------------------------------------- */
static uint64_t *terms_u64           = NULL;  /* sequence values (as 64‑bit). */
static size_t   terms_len            = 0;
static size_t   terms_cap            = 0;

static char **sig_arr                = NULL;  /* unique signatures seen. */
static size_t  sig_len               = 0;
static size_t  sig_cap               = 0;

static bool   *is_composite          = NULL;  /* sieve marks. */
static uint64_t *prime_list          = NULL;  /* list of primes up to sieve_limit. */
static size_t  prime_cnt             = 0;     /* number of primes. */
static idx_t   sieve_limit           = 0;     /* current sieve maximum. */
static size_t  prime_processed       = 0;     /* index of next prime to consider */

/* ---------- grow dynamic arrays ----------------------------------- */
static void grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? (terms_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(terms_u64, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    terms_u64 = tmp;
    terms_cap = new_cap;
}

static void grow_sigs(size_t need)
{
    if (need <= sig_cap) return;
    size_t new_cap = sig_cap ? (sig_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    char **tmp = (char **)realloc(sig_arr, new_cap * sizeof(char *));
    if (!tmp) abort();
    sig_arr = tmp;
    sig_cap = new_cap;
}

/* ---------- sieve ------------------------------------------------- */
static void rebuild_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* free previous data */
    free(is_composite);
    free(prime_list);

    sieve_limit = limit;
    size_t sz = (size_t)limit + 1;
    is_composite = (bool *)calloc(sz, sizeof(bool));
    if (!is_composite) abort();

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[(size_t)j] = true;
        }
    }

    /* count primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i]) prime_list[idx++] = (uint64_t)i;
}

/* ---------- signature of n (sorted exponent multiset) ------------- */
static char *signature_of(uint64_t n)
{
    if (n == 1) {
        char *s = (char *)malloc(1);
        if (!s) abort();
        s[0] = '\0';
        return s;
    }

    uint8_t exps[64];
    size_t ecnt = 0;
    uint64_t temp = n;

    /* factor by trial division using prime list */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint8_t e = 0;
            do {
                temp /= p;
                ++e;
            } while (temp % p == 0);
            exps[ecnt++] = e;
        }
    }
    if (temp > 1) /* remaining prime factor */
        exps[ecnt++] = 1;

    /* sort exponents ascending (smallest bubble sort sufficient). */
    for (size_t i = 0; i < ecnt; ++i)
        for (size_t j = i + 1; j < ecnt; ++j)
            if (exps[j] < exps[i]) {
                uint8_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }

    char *s = (char *)malloc(ecnt + 1);
    if (!s) abort();
    for (size_t i = 0; i < ecnt; ++i)
        s[i] = (char)exps[i];
    s[ecnt] = '\0';
    return s;
}

/* ---------- signature lookup ------------------------------------- */
static bool sig_exists(const char *s)
{
    for (size_t i = 0; i < sig_len; ++i)
        if (strcmp(sig_arr[i], s) == 0) return true;
    return false;
}

static void sig_add(const char *s)
{
    grow_sigs(sig_len + 1);
    char *copy = (char *)malloc(strlen(s) + 1);
    if (!copy) abort();
    strcpy(copy, s);
    sig_arr[sig_len++] = copy;
}

/* ---------- add term --------------------------------------------- */
static void term_add(uint64_t p)
{
    grow_terms(terms_len + 1);
    terms_u64[terms_len++] = p;
}

/* ---------- ensure enough terms are computed --------------------- */
static void ensure_terms(idx_t n)
{
    if (terms_len > n) return;

    /* start with a modest limit if not yet built */
    if (sieve_limit == 0)
        rebuild_sieve(100);

    while (terms_len <= (size_t)n) {
        if (prime_processed >= prime_cnt) {
            /* need more primes – double the limit */
            idx_t new_limit = (sieve_limit == 0) ? 200 : sieve_limit * 2;
            rebuild_sieve(new_limit);
        }

        uint64_t p = prime_list[prime_processed++];
        uint64_t m = p - 1;
        char *sig = signature_of(m);
        if (!sig_exists(sig)) {
            term_add(p);
            sig_add(sig);
        }
        free(sig);
    }
}

/* ---------- public function ------------------------------------- */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return (seq_t)terms_u64[n];
}

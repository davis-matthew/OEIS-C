/*
 * A084197  Primes p such that there exists at least one prime q<p with q+1
 *          dividing p+1.
 * https://oeis.org/A084197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 5504, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084197
   Prime p such that there exists at least one prime q < p with (q+1)|(p+1).
   The function computes the n-th element by searching primes until the
   needed amount is found.  It memoises found primes and sequence terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- internal static storage ---------- */
static uint64_t *prime_arr = NULL;
static size_t    prime_len = 0, prime_cap = 0;

static seq_t   *term_arr  = NULL;
static size_t   term_len  = 0, term_cap  = 0;

static uint64_t next_candidate = 3; /* next odd number to test for primality */

/* ---------- helpers for dynamic arrays ---------- */
static void ensure_prime_cap(void)
{
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1U : 64U;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_arr = tmp;
        prime_cap = new_cap;
    }
}

static void ensure_term_cap(void)
{
    if (term_len >= term_cap) {
        size_t new_cap = term_cap ? term_cap << 1U : 64U;
        seq_t *tmp = (seq_t *)realloc(term_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        term_arr = tmp;
        term_cap = new_cap;
    }
}

/* ---------- prime utilities ---------- */
static void init_primes(void)
{
    if (prime_arr) return;              /* already initialised */
    ensure_prime_cap();
    prime_arr[prime_len++] = 2ULL;       /* first prime */
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0ULL) return n == 2ULL;

    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_arr[i];
        if (p > n / p) break;           /* avoid overflow on p*p */
        if (n % p == 0ULL) return false;
    }
    return true;
}

static uint64_t generate_next_prime(void)
{
    uint64_t cand = next_candidate;
    for (;;) {
        if (cand == 2ULL) {
            next_candidate = 3ULL;
            ensure_prime_cap();
            prime_arr[prime_len++] = cand;
            return cand;
        }
        if (is_prime_uint64(cand)) {
            next_candidate = cand + 2ULL;   /* next odd candidate */
            ensure_prime_cap();
            prime_arr[prime_len++] = cand;
            return cand;
        }
        cand += 2ULL;  /* stay on odds */
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0; safe guard */

    init_primes();          /* ensures prime_arr[0] = 2 */

    if (!term_arr) {
        ensure_term_cap();
    }

    /* generate terms until at least n of them are stored */
    while (term_len < (size_t)n) {
        uint64_t p = generate_next_prime();               /* new prime */
        seq_t p_plus = (seq_t)p + (seq_t)1;                /* p+1 */

        bool good = false;
        /* check all primes q < p */
        for (size_t i = 0; i + 1 < prime_len; ++i) {
            seq_t d = (seq_t)prime_arr[i] + (seq_t)1;     /* q+1 */
            if (p_plus % d == 0) { good = true; break; }
        }

        if (good) {
            ensure_term_cap();
            term_arr[term_len++] = (seq_t)p;
        }
    }

    return term_arr[(size_t)n - 1];
}

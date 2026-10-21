/*
 * A060462  Integers k such that k! is divisible by k*(k+1)/2.
 * https://oeis.org/A060462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 2001, u32 n <= 2001, u64 n <= 2001,
 *            u128 n <= 2001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A060462: integers k such that k! is divisible by k*(k+1)/2. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- global data ---------- */
static uint32_t *spf = NULL;        /* smallest prime factor for each n ≤ sieve_limit */
static uint32_t *exp_arr = NULL;    /* factorial prime exponents: exp_arr[p] for prime p */
static idx_t sieve_limit = 0;       /* current extent of the sieve */
static bool init_done = false;      /* one‑time initialization flag */

static idx_t *cached_k = NULL;      /* cached k values (indices of sequence) */
static size_t cached_cap = 0;       /* allocated capacity of cached_k */
static size_t cached_size = 0;      /* number of cached terms */
static idx_t current_k = 0;         /* largest k processed so far */

/* ---------- helper: ensure sieve capacity ---------- */
static void extend_sieve(idx_t need)
{
    if (need <= sieve_limit) return;
    idx_t new_limit = sieve_limit == 0 ? 1024 : sieve_limit;
    while (new_limit < need) new_limit <<= 1;

    uint32_t *new_spf = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= new_limit; ++i) new_spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (new_spf[i] == i) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                if (new_spf[j] == j) new_spf[j] = (uint32_t)i;
        }
    }

    uint32_t *new_exp = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= new_limit; ++i) new_exp[i] = 0;

    if (exp_arr) {
        for (idx_t i = 0; i <= sieve_limit; ++i) new_exp[i] = exp_arr[i];
        free(spf);
        free(exp_arr);
    }

    spf = new_spf;
    exp_arr = new_exp;
    sieve_limit = new_limit;
}

/* ---------- helper: add factorization of x to factorial exponents ---------- */
static void add_factorization(idx_t x)
{
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        exp_arr[p] += cnt;
    }
}

/* ---------- helper: check divisibility for given k ---------- */
static bool check_k(idx_t k)
{
    /* factors of k */
    idx_t t = k;
    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t cnt = 0;
        while (t % p == 0) { t /= p; ++cnt; }
        if (exp_arr[p] < cnt) return false;
    }
    /* factors of k+1, subtract one for the division by 2 */
    idx_t m = k + 1;
    while (m > 1) {
        uint32_t p = spf[m];
        uint32_t cnt = 0;
        while (m % p == 0) { m /= p; ++cnt; }
        if (p == 2) {
            if (cnt == 0) continue;
            --cnt;
        }
        if (exp_arr[p] < cnt) return false;
    }
    return true;
}

/* ---------- helper: ensure capacity for cached_k ------------ */
static void ensure_cached(size_t needed)
{
    if (needed <= cached_cap) return;
    size_t new_cap = cached_cap < 8 ? 8 : cached_cap * 2;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t*)realloc(cached_k, new_cap * sizeof(idx_t));
    cached_k = tmp;
    cached_cap = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for non‑positive indices */

    if (!init_done) {
        extend_sieve(1024);
        ensure_cached(16);
        init_done = true;
    }

    if (cached_size >= n) return (seq_t)cached_k[n - 1];

    while (cached_size < n) {
        ++current_k;
        if (current_k + 1 > sieve_limit) extend_sieve(current_k + 1);
        add_factorization(current_k);
        if (check_k(current_k)) {
            ensure_cached(cached_size + 1);
            cached_k[cached_size++] = current_k;
        }
    }
    return (seq_t)cached_k[n - 1];
}

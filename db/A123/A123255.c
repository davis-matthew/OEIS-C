/*
 * A123255  Numbers k such that 4k+1, 4k+2, and 4k+3 are all semiprimes.
 * https://oeis.org/A123255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 829, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123255: a(n) is the k such that 4k+1, 4k+2 and 4k+3 are
 * all semiprimes.  This implementation generates terms on demand
 * and memoises them. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Prime utilities provided by the framework */
static uint64_t *primes = NULL;      /* list of primes */
static size_t  primes_len = 0;
static size_t  primes_cap = 0;

/* Grow the primes array when necessary */
static void grow_primes(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

/* Add a new prime to the list */
static inline void add_prime(uint64_t p)
{
    grow_primes(primes_len + 1);
    primes[primes_len++] = p;
}

/* Integer square‑root for idx_t values (Newton iteration) */
static inline idx_t isqrt_idx(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Ensure all primes ≤ limit are generated.  Uses trial division by
 * the already known primes. */
static void ensure_primes_limit(uint64_t limit)
{
    if (primes_len > 0 && primes[primes_len - 1] >= limit)
        return;

    uint64_t start = primes_len ? primes[primes_len - 1] + 1 : 2;
    for (uint64_t cand = start; cand <= limit; ++cand) {
        bool is_prime = true;
        uint64_t r = (uint64_t)isqrt_idx(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= r; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(cand);
    }
}

/* Test whether v is a semiprime (product of exactly two prime
 * factors, counting multiplicity). */
static inline bool is_semiprime(idx_t v)
{
    if (v < 4)
        return false;

    int cnt = 0;
    idx_t m = v;
    for (size_t i = 0; i < primes_len &&
         (idx_t)primes[i] * (idx_t)primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        while (m % p == 0) {
            ++cnt;
            if (cnt > 2)
                return false;
            m /= p;
        }
    }
    if (m > 1)
        ++cnt;
    return cnt == 2;
}

/* Sequence storage and memoisation */
static seq_t *terms = NULL;       /* cached a(n) values (k) */
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t  next_k   = 0;       /* next k to examine */

/* Grow the terms array when needed */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  The public sequence function                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined, but safe */

    /* Ensure we have at least n terms computed */
    while (terms_len < n) {
        /* Compute the maximum number that will be factored for this k */
        idx_t max_num = 4 * next_k + 3;
        uint64_t prime_limit = (uint64_t)isqrt_idx(max_num) + 1;
        ensure_primes_limit(prime_limit);

        if (is_semiprime(4 * next_k + 1) &&
            is_semiprime(4 * next_k + 2) &&
            is_semiprime(max_num)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}

/*
 * A046097  Values of n for which binomial(2n-1, n) is squarefree.
 * https://oeis.org/A046097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = the n-th integer m (m ≥ 1) such that C(2m-1,m) is
 *            squarefree
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes_upto, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Compute v_p(n!) by Legendre's formula. */
static int exp_in_fact(idx_t n, uint64_t p)
{
    int e = 0;
    for (idx_t t = n; t; t = t / p)
        e += t / p;
    return e;
}

/* Test whether C(2m-1,m) is squarefree using prime exponents. */
static bool squarefree_binom(idx_t m)
{
    if (m == 0)
        return false;   /* not needed as m ≥ 1 */

    idx_t limit = 2 * m - 1;

    /* Lazily maintain a prime table up to the current limit. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t primes_upto = 0;

    if (limit > primes_upto) {
        uint64_t *old = primes;
        size_t old_cnt = prime_cnt;
        build_primes_upto(limit, &primes, &prime_cnt);
        primes_upto = limit;
        /* The build function takes care of freeing the old array if needed. */
        (void)old; (void)old_cnt;    /* silence unused warnings */
    }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit)
            break;
        int e = exp_in_fact(limit, p) - exp_in_fact(m, p) - exp_in_fact(m - 1, p);
        if (e >= 2)
            return false;
    }
    return true;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* index 0 is undefined, return 0 for safety */

    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;
    static idx_t next_to_check = 1;

    /* Initialise the cache lazily. */
    if (cache == NULL) {
        cap = 64;
        cache = seq_alloc(cap);
    }

    /* Generate terms until we have at least n of them. */
    while (cached < n) {
        if (squarefree_binom(next_to_check)) {
            /* Grow cache if needed. */
            if (cached == cap) {
                size_t new_cap = cap << 1;
                cache = seq_realloc(cache, cap, new_cap);
                cap = new_cap;
            }
            cache[cached++] = (seq_t)next_to_check;
        }
        ++next_to_check;
    }

    return cache[n - 1];
}

/*
 * A145032  If t(n) is the maximal triangular number not exceeding n, then
 *          a(n) is the n-th prime for which a(n)-t(a(n)) is a triangular
 *          number.
 * https://oeis.org/A145032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A145032: primes p for which p – t(p) is triangular.
 *  t(p) is the largest triangular number ≤ p.
 *  The algorithm generates primes incrementally and stores the
 *  qualifying primes in a memoized cache.  Complexity is
 *  roughly O(n * sqrt(p)) for the n‑th term; for n ≤ 10 000
 *  it runs well under the time limit.  It works for any
 *  integer type seq_t and the standard integer type idx_t
 *  (typically uint64_t). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

/* simple deterministic primality test for uint64_t */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Memoization cache for the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t last_checked = 1;   /* candidate initialization */

    /* Extend the cache until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* Find next prime */
        uint64_t p = 0;
        do {
            last_checked++;
            if (is_prime_uint64(last_checked))
                p = last_checked;
        } while (p == 0);

        seq_t sp = (seq_t)p;                     /* cast to seq_t */

        /* Compute largest triangular ≤ p:
         *   t = floor((sqrt(8p+1)-1)/2)
         *   tTri = t*(t+1)/2
         */
        seq_t eightp1 = (seq_t)8 * sp + 1;
        seq_t sqrt8p1 = floor_sqrt(eightp1);
        seq_t t = (sqrt8p1 - 1) / 2;
        seq_t tTri = t * (t + 1) / 2;

        /* r = p - tTri; check if r is triangular by verifying
         * that 8r+1 is a perfect square.
         */
        seq_t r = sp - tTri;
        seq_t eightr1 = (seq_t)8 * r + 1;
        seq_t sqrt8r1 = floor_sqrt(eightr1);
        if (sqrt8r1 * sqrt8r1 == eightr1) {
            /* r is triangular → p is part of the sequence */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                if (new_cap < cache_size + 1) new_cap = cache_size + 1;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = sp;
        }
    }

    return cache[n - 1];
}

/*
 * A133564  a(1)=1. a(n+1) = sum{k=isolated divisors of n} a(k). An isolated
 *          divisor, k, of n is a positive divisor of n where neither (k-1)
 *          nor (k+1) divides n.
 * https://oeis.org/A133564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = sum of a(k) for all isolated
 *            divisors k of n‑1 (a divisor k of m is isolated if neither k‑1
 *            nor k+1 divides m)
 * Range:     u8 n <= 42, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Recursive memoised evaluation of A133564 */
seq_t a(idx_t n)
{
    /* static memoisation tables, 1‑based indexing */
    static seq_t *cache    = NULL;   /* stored values a(i)   */
    static bool *computed = NULL;   /* flags telling if cache[i] valid */
    static size_t cap     = 0;      /* allocated size of the arrays */

    /* ensure capacity for index n (1‑based) */
    if ((size_t)n >= cap) {
        size_t newcap = cap == 0 ? 16 : cap;
        while (newcap <= (size_t)n) newcap <<= 1;

        /* realloc cache entries, keeping previous values */
        seq_t *tmp_cache    = (seq_t *)realloc(cache,    newcap * sizeof(seq_t));
        bool   *tmp_computed = (bool *)realloc(computed, newcap * sizeof(bool));

        if (tmp_cache    == NULL || tmp_computed == NULL) {
            /* on allocation failure, abort (cannot report error in OEIS eval) */
            abort();
        }

        /* initialize new entries as 'not computed' */
        for (size_t i = cap; i < newcap; ++i) {
            tmp_computed[i] = false;
        }

        cache    = tmp_cache;
        computed = tmp_computed;
        cap      = newcap;
    }

    /* return cached result if already known */
    if (computed[(size_t)n]) {
        return cache[(size_t)n];
    }

    /* base case */
    if (n == 1) {
        cache[1] = (seq_t)1;
        computed[1] = true;
        return cache[1];
    }

    /* work with the integer m = n-1 */
    uint64_t m = (uint64_t)(n - 1);

    /* factorise m (trial division is enough for the limits here) */
    uint64_t primes[20];
    int      exps[20];
    size_t   pcnt = 0;
    uint64_t rem   = m;
    if (rem % 2 == 0) {
        int e = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt]   = e;
        ++pcnt;
    }
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (rem > 1) {
        primes[pcnt] = rem;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* generate all divisors of m */
    uint64_t divisors[256];
    size_t   divisor_count = 1;
    divisors[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];
        size_t   curcnt = divisor_count;
        uint64_t mul = 1;
        for (int k = 1; k <= e; ++k) {
            mul *= p;
            for (size_t j = 0; j < curcnt; ++j) {
                divisors[divisor_count++] = divisors[j] * mul;
            }
        }
    }

    /* sum over isolated divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < divisor_count; ++i) {
        uint64_t d = divisors[i];
        bool isolated = false;
        if (d == 1) {
            /* only check d+1 */
            isolated = (m % 2 != 0);
        } else if (d == m) {
            /* only check d-1 */
            isolated = (m % (m - 1) != 0);
        } else {
            isolated = (m % (d - 1) != 0) && (m % (d + 1) != 0);
        }
        if (isolated) {
            idx_t k = (idx_t)d;
            sum += a(k);           /* recursion with memoisation */
        }
    }

    cache[(size_t)n]    = sum;
    computed[(size_t)n] = true;
    return sum;
}

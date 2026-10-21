/*
 * A088242  Values of x, where x^2 + xy + y^2 = p (x<y) is a prime of the form
 *          6n + 1 (A002476).
 * https://oeis.org/A088242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9007, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A088242: a(n) = the x in the unique representation
 *  p = x² + x y + y²,  x < y,  where p is the n‑th prime ≡ 1 (mod 6).
 *  The function computes the n‑th term by successively locating
 *  primes of the form 6k+1 and determining the corresponding x
 *  via integer arithmetic only.
 *  It memoises results so that any order of calls is handled
 *  efficiently, and constraints on seq_t overflow are respected
 *  by performing all intermediate calculations in 64‑bit
 *  integer arithmetic (unsigned __int128 for safety).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static inline idx_t isqrt_u64(idx_t n)
{
    /* Returns ⌊√n⌋ for unsigned 64‑bit n using binary search.
       The method is safe for all values that fit into idx_t. */
    idx_t low = 0, high = 1;
    /* Expand high until high² > n */
    while ((__int128)high * high <= n)
        high <<= 1;
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for indices < 1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms stored (cache[0] is a(1)) */
    static idx_t next_candidate = 7;   /* first prime candidate of form 6k+1 */

    /* Grow cache until we have at least n terms */
    while (cache_len < n)
    {
        /* Find next prime of form 6k+1 */
        idx_t p = next_candidate;
        while (!is_prime(p))
            p += 6;

        /* Compute the corresponding x for this prime p */
        seq_t x_val = 0;
        {
            /* Compute upper bound for x: 4p - 3x² ≥ 0  ⇒  x² ≤ 4p/3 */
            idx_t max = (idx_t)(((__int128)4 * p) / 3);
            idx_t xmax = isqrt_u64(max);

            for (idx_t x = 1; x <= xmax; ++x)
            {
                idx_t xx = x;
                /* Check 4p - 3x² is non‑negative */
                __int128 tmp = (__int128)4 * p - (__int128)3 * xx * xx;
                if (tmp < 0)
                    break;

                idx_t D = (idx_t)tmp;
                idx_t s = isqrt_u64(D);
                if ((s * (idx_t)s) != D)
                    continue;          /* D not a perfect square */

                if ((s - xx) & 1)
                    continue;          /* y would not be integer */

                idx_t y = (s - xx) / 2;
                if (y >= xx && y > 0)
                {
                    x_val = (seq_t)xx;
                    break;
                }
            }
            /* For primes ≡ 1 (mod 3) a representation always exists */
        }

        /* Store result in cache, reallocating if needed */
        if (cache_len == 0)
        {
            cache = (seq_t *)malloc(64 * sizeof(seq_t));
            if (!cache)
                abort();   /* allocation failure */
        }
        else if (cache_len % 64 == 0)
        {
            seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 64) * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
        }

        cache[cache_len++] = x_val;
        next_candidate = p + 6;   /* next candidate for the following prime */
    }

    return cache[n - 1]; /* a(1) is stored in cache[0] */
}

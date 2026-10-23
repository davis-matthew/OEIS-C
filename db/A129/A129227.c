/*
 * A129227  a(n) is that prime number, p, less than n*Pi such that n*Pi/p has
 *          the smallest fractional part.
 * https://oeis.org/A129227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime p < n*π such that the fractional part of n*π/p is minimal. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Use a high‑precision constant for π. */
    const double pi = 3.14159265358979323846264338327950288419716939937510L;

    /* Upper bound for primes to examine. */
    const idx_t limit = (idx_t)floor((double)n * pi);

    /* Simple sieve of Eratosthenes to collect primes up to limit. */
    bool *isComp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!isComp) return (seq_t)0;          /* allocation failure */

    size_t cap   = limit / 10 + 10;        /* rough initial capacity */
    idx_t *primes = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!primes) { free(isComp); return (seq_t)0; }

    size_t prcnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!isComp[(size_t)i])      /* i is prime */
        {
            if (prcnt == cap)        /* grow array if needed */
            {
                cap <<= 1;           /* double the capacity */
                idx_t *tmp = (idx_t *)realloc(primes, cap * sizeof(idx_t));
                if (!tmp) { free(primes); free(isComp); return (seq_t)0; }
                primes = tmp;
            }
            primes[prcnt++] = i;

            if (i * i <= limit)
            {
                for (idx_t j = i * i; j <= limit; j += i)
                    isComp[(size_t)j] = true;
            }
        }
    }

    /* Find the prime with minimal fractional part. */
    double best_frac = 1.0;          /* value in [0,1) */
    seq_t best_p = 0;
    const double eps = 1e-12;        /* tolerance for rounding errors */

    for (size_t idx = 0; idx < prcnt; ++idx)
    {
        double frac = fmod((double)n * pi / (double)primes[idx], 1.0);
        if (frac < 0.0) frac += 1.0;  /* safety for negative rounding */
        if (frac + eps < best_frac)
        {
            best_frac = frac;
            best_p = primes[idx];
        }
    }

    free(isComp);
    free(primes);

    return best_p;
}

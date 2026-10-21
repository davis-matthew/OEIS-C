/*
 * A072048  Number of divisors of the squarefree numbers: tau(A005117(n)).
 * https://oeis.org/A072048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^Ω(A005117(n)), where A005117(n) is the nth square‑free
 *            integer and Ω(m) is the number of distinct prime factors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   omega_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans integers, marks those divisible by a square,
   counts square‑free numbers up to an estimate of the desired term,
   factors the found number by trial division, and returns 2^ω(m).  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/omega_of.h"

static size_t int_sqrt(size_t x)
{
    size_t r = (size_t)sqrt((double)x);
    while ((r+1)*(r+1) <= x) ++r;
    while (r*r > x) --r;
    return r;
}

/* Find the n-th square‑free integer.  */
static size_t nth_squarefree(idx_t n)
{
    if (n == 0) return 0;           /* never used, but safe */

    /* Rough upper bound: square‑free density is 6/π² ≈ 0.608.
       We use 3n + 100 to be safely above the real value.  */
    size_t bound = (size_t)n * 3 + 100;
    size_t limit   = int_sqrt(bound) + 1;

    /* Sieve to mark primes up to limit. */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t k = p * p; k <= limit; k += p)
                is_prime[k] = false;

    /* Mark numbers up to bound that are divisible by a square.  */
    bool *is_sf = (bool *)malloc((bound + 1) * sizeof(bool));
    for (size_t i = 1; i <= bound; ++i) is_sf[i] = true;
    for (size_t p = 2; p <= limit; ++p)
        if (is_prime[p])
        {
            size_t sq = p * p;
            for (size_t k = sq; k <= bound; k += sq)
                is_sf[k] = false;
        }

    /* Count square‑free numbers until we reach the n-th.  */
    size_t cnt = 0;
    size_t result = 0;
    for (size_t i = 1; i <= bound; ++i)
    {
        if (is_sf[i])
        {
            ++cnt;
            if (cnt == (size_t)n)
            {
                result = i;
                break;
            }
        }
    }

    free(is_prime);
    free(is_sf);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* guard, offset is 1 */
    size_t num = nth_squarefree(n);            /* A005117(n) */
    seq_t omega = omega_of(num);               /* Ω(num) */
    seq_t ans = 1;
    for (seq_t i = 0; i < omega; ++i)
        ans <<= 1;                             /* multiply by 2 per prime factor */
    return ans;
}

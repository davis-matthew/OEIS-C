/*
 * A093074  Greatest prime factor of n and its direct neighbors.
 * https://oeis.org/A093074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{LPF(n‑1), LPF(n), LPF(n+1)} , where LPF(m) is the
 *            largest prime divisor of m; by convention a(1) = 2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* external type aliases are supplied by the build
   (idx_t is an unsigned integer type, seq_t is an unsigned
   integer type of unspecified size).  */
seq_t a(idx_t n)
{
    /* Handle the offset of the sequence. */
    if (n <= 1)
        return (seq_t)2;

    /* We need largest prime factors for n‑1, n and n+1.
       The easiest safe way is to run a small sieve up to n+1
       each time the function is called.  For n ≤ 10 000 this is
       trivial and stays well within the time limits. */

    idx_t limit = n + 1;                 /* inclusive */
    size_t sz   = (size_t)limit + 1;     /* array size */

    uint64_t *lpf = (uint64_t *)calloc(sz, sizeof(uint64_t));
    if (!lpf)                            /* allocation failure – safe fallback */
        return (seq_t)0;

    /* Sieve of Eratosthenes that stores the largest prime factor
       in lpf[i] for each i≥2.  Largest prime factor is written
       last, because later primes overwrite earlier ones. */
    for (idx_t p = 2; p <= limit; ++p) {
        if (lpf[p] == 0) {               /* p is prime */
            for (idx_t m = p; m <= limit; m += p)
                lpf[m] = p;
        }
    }

    /* Fetch the three candidate values.  For n>1, n−1 ≥ 1. */
    uint64_t v1 = lpf[n - 1];
    uint64_t v2 = lpf[n];
    uint64_t v3 = lpf[n + 1];

    /* Compute the maximum. */
    uint64_t mx = v1;
    if (v2 > mx) mx = v2;
    if (v3 > mx) mx = v3;

    free(lpf);
    return (seq_t)mx;
}

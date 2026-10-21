/*
 * A228775  a(n) is the maximal k>=1 such that nextprime(j*n)<=(j+1)*n,
 *          j=1,...,k.
 * https://oeis.org/A228775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): maximal k such that for all j = 1..k,
   next_prime(j*n) <= (j+1)*n. */

#include "../utils/next_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t j = 1;
    for (;;) {
        /* Prevent overflow of j*n when using 64‑bit arithmetic. */
        if (n != 0 && j > UINT64_MAX / n)
            break;

        uint64_t base  = j * n;        /* j*n  */
        uint64_t upper = base + n;     /* (j+1)*n */

        uint64_t p = next_prime(base); /* smallest prime > j*n */

        if (p <= upper)
            j++;                /* condition holds for this j, try next */
        else
            break;              /* first j where condition fails */
    }
    return (seq_t)(j - 1);
}

/*
 * A091087  a(n) = floor(r*n) + floor(n/r), where r=sqrt(2).
 * https://oeis.org/A091087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2)*n) + floor(n/sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses only integer arithmetic and the provided floor_sqrt */

/* The sequence is defined for all non‑negative integer indices n.
 * For each n, a(n) is the sum of two integer square‑root terms:
 *   floor(sqrt(2) * n)   == floor( sqrt( 2 * n * n ) )
 *   floor(n / sqrt(2))   == floor( sqrt( n * n / 2 ) )
 * The function below evaluates these two floor‑square‑roots using
 * the `floor_sqrt` helper and returns their sum.  No floating point
 * or external libraries are required beyond the standard types. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* promote index to seq_t */
    seq_t nn = sn * sn;            /* n*n, holds in seq_t for the chosen build */

    /* floor(sqrt(2) * n) = floor(sqrt(2*n*n)) */
    seq_t t1 = floor_sqrt(nn << 1);          /* multiply by 2 safely in seq_t */

    /* floor(n / sqrt(2)) = floor(sqrt(n*n/2)) */
    seq_t t2 = floor_sqrt(nn >> 1);          /* divide by 2 (integer shift) */

    return t1 + t2;
}

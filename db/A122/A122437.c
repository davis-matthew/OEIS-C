/*
 * A122437  Allowable values of the "dropping time" of the Collatz (3x+1)
 *          iteration.
 * https://oeis.org/A122437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n + floor((n-1) * log2(3))
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    /* log2(3) with sufficient precision (≈1.584962500721156…) */
    static const double LOG2_3 = 1.584962500721156181453125;
    double prod = (double)(n - 1) * LOG2_3;     /* (n‑1)·log2(3) */
    unsigned long long frac = (unsigned long long)prod; /* floor */
    return (seq_t)(n + frac);
}

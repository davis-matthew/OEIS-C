/*
 * A184921  a(n) = n+[r*n/s]+[t*n/s]+[u*n/s], where []=floor and r=2^(1/2),
 *          s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A184921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(2−√2)n⌋ + ⌊√2 n⌋ + ⌊(2√2−1)n⌋ Simplification: a(n)
 *            = ⌊(2+2√2)n⌋ − 1 Further simplification: a(n) = ⌊√(8n²)⌋ + 2n −
 *            1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for any seq_t type. */
static seq_t isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;              /* convert index to seq_t */
    seq_t temp = sn * sn;             /* n² */
    temp *= (seq_t)8;                 /* 8·n² */
    seq_t sqrt_val = isqrt(temp);     /* floor(sqrt(8·n²)) */
    seq_t result = sqrt_val + sn * (seq_t)2 - (seq_t)1;
    return result;
}

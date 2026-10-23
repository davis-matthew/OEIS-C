/*
 * A191610  Possible number of trailing zeros in k!.
 * https://oeis.org/A191610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} floor((n-1)/5^k)
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for UINT64_MAX */

seq_t a(idx_t n)
{
    /* The sequence starts at index 1. */
    if (n == 0) return 0;

    idx_t m = n - 1;         /* m = n-1 */
    seq_t sum = 0;           /* result accumulator */
    idx_t p = 1;             /* current power of 5 */

    while (p <= m) {
        sum += (seq_t)(m / p);              /* add floor(m/p) */
        if (p > UINT64_MAX / 5) break;      /* avoid overflow on next multiply */
        p *= 5;                             /* next power of 5 */
    }
    return sum;
}

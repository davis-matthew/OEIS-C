/*
 * A186852  Number of 3-step knight's tours on an (n+2) X (n+2) board summed
 *          over all starting positions.
 * https://oeis.org/A186852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*n^2 - 56*n - 8 for n > 1, with a(1) = 16
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (sn == (seq_t)1)
        return (seq_t)16;

    seq_t res = sn * sn;           /* n^2 */
    res = res * (seq_t)56;         /* 56*n^2 */
    res = res - (sn * (seq_t)56);  /* -56*n */
    res = res - (seq_t)8;          /* -8 */
    return res;
}

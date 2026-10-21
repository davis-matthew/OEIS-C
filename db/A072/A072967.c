/*
 * A072967  Least k>n such that the last digit of k^k is the same as the last
 *          digit of n^n.
 * https://oeis.org/A072967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n + A072968(n) * A072968 is periodic with period 20: (10,
 *            16, 14, 2, 10, 2, 6, 4, 10, 10, * 10, 2, 14, 2, 10, 8, 6, 4, 10,
 *            10). * Thus a(n+20) = a(n) + 20 and a(n) = n + delta[(n-1) mod
 *            20]
 * Range:     u8 n <= 241, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t delta[20] = {
    10, 16, 14, 2, 10, 2, 6, 4, 10, 10,
    10, 2, 14, 2, 10, 8, 6, 4, 10, 10
};

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    idx_t idx = (n - 1) % 20;                /* periodic index (0‑based) */
    seq_t result = (seq_t)n;                 /* start with n */
    result += (seq_t)delta[idx];             /* add the periodic offset */
    return result;
}

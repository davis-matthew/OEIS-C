/*
 * A083083  A diagonal of number array A083075.
 * https://oeis.org/A083083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+2)*(2n+7)^n + 1)/(n+3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t N      = (seq_t)n;
    seq_t base   = (seq_t)(2 * N + 7);
    seq_t pow_val = big_pow(base, N);          /* (2n+7)^n   */
    seq_t num = (N + 2) * pow_val + 1;         /* (n+2)*(2n+7)^n + 1  */
    return num / (N + 3);                      /* divide by n+3   */
}

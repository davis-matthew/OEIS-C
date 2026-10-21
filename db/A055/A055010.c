/*
 * A055010  a(0) = 0; for n > 0, a(n) = 3*2^(n-1) - 1.
 * https://oeis.org/A055010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 3*2^(n-1) - 1 (a(0) = 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t res = (seq_t)3;      /* start with 3   */
    /* multiply by 2 exactly (n-1) times – works for all seq_t types */
    for (idx_t i = 1; i < n; ++i)
        res <<= 1;            /* res = res * 2 */
    res -= (seq_t)1;          /* final subtraction */
    return res;
}

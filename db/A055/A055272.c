/*
 * A055272  First differences of 7^n (A000420).
 * https://oeis.org/A055272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = 6*7^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)6;          /* 6*7^(n-1) = 6 * 7 * 7 * ... */
    for (idx_t i = 1; i < n; ++i)      /* multiply by 7 (n-1) times */
        result *= (seq_t)7;
    return result;
}

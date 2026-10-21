/*
 * A062709  a(n) = 2^n + 3.
 * https://oeis.org/A062709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                /* start with 2^0 */
    for (idx_t i = 0; i < n; ++i) {
        val *= (seq_t)2;          /* multiply by 2 n times */
    }
    val += (seq_t)3;              /* add the constant 3 */
    return val;
}

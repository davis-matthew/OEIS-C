/*
 * A013836  a(n) = 5^(5*n + 3).
 * https://oeis.org/A013836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(5*n + 3) (a(0) = 125)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  5^(5*n+3) = 125 * 3125^n  */
    seq_t val = (seq_t)125;          /* 5^3 */
    for (idx_t i = 0; i < n; ++i)    /* multiply by 3125 n times */
        val = val * (seq_t)3125;
    return val;
}

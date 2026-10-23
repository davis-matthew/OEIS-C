/*
 * A126284  a(n) = 5*2^n - 4*n - 5.
 * https://oeis.org/A126284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - 4*n - 5
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* outside defined offset; arbitrary value */

    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i)
        val *= 2;

    val *= 5;
    val -= (seq_t)4 * (seq_t)n;
    val -= 5;

    return val;
}

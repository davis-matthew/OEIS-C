/*
 * A051179  a(n) = 2^(2^n) - 1.
 * https://oeis.org/A051179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(2^n) - 1 (recurrence: a(0) = 1, a(n) = (a(n-1)+1)^2 -
 *            1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^(2^n) can be built by repeated squaring
       starting from 2 and squaring n times:
       after i squarings: 2^(2^i). */
    seq_t val = (seq_t)2;
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * val;   /* val = 2^(2^(i+1)) */
    }
    val -= 1;               /* final result = 2^(2^n) - 1 */
    return val;
}

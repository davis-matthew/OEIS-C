/*
 * A079583  a(n) = 3*2^n - n - 2.
 * https://oeis.org/A079583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - n - 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 0; i < n; ++i)
        val *= 2;          /* val = 2^n */

    val *= 3;              /* val = 3*2^n */
    val -= (seq_t)n;       /* subtract n */
    val -= 2;              /* subtract 2 */
    return val;
}

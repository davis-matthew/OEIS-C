/*
 * A056547  a(n) = 6*n*a(n-1) + 1 with a(0)=1.
 * https://oeis.org/A056547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Multiply by 6*i and add 1 */
        val = val * ((seq_t)(6 * i)) + (seq_t)1;
    }
    return val;
}

/*
 * A033578  a(n) = (3*n - 1)*(4*n - 1).
 * https://oeis.org/A033578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1)*(4*n-1)
 * Range:     u8 n <= 4, u16 n <= 74, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 to avoid unsigned underflow */
    if (n == 0) {
        return 1;
    }
    seq_t term1 = ((seq_t)3 * n) - 1;
    seq_t term2 = ((seq_t)4 * n) - 1;
    return term1 * term2;
}

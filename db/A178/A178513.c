/*
 * A178513  Partial sums of 80^n.
 * https://oeis.org/A178513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;           /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        result = 80 * result + 1;
    return result;
}

/*
 * A082035  a(n) = (4n^2+2n+1) * n!.
 * https://oeis.org/A082035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n^2 + 2*n + 1) * n!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;          /* factorial part */
    for (idx_t i = 1; i <= n; ++i)
        res *= (seq_t)i;    /* res = i! */

    seq_t nn = (seq_t)n;
    seq_t mult = 4 * nn * nn + 2 * nn + 1; /* (4n^2 + 2n + 1) */

    return res * mult;
}

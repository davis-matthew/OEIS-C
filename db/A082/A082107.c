/*
 * A082107  Diagonal sums of number array A082105.
 * https://oeis.org/A082107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^5 + 20*n^3 + 9*n + 30)/30
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid overflow of intermediate products */
    unsigned __int128 nn = (unsigned __int128)n;   /* n itself          */
    unsigned __int128 n2 = nn * nn;                /* n^2               */
    unsigned __int128 n3 = n2 * nn;                /* n^3               */
    unsigned __int128 n5 = n3 * n2;                /* n^5               */
    unsigned __int128 val = n5 + (unsigned __int128)20 * n3
                          + (unsigned __int128)9 * nn + 30;
    val /= 30;                                     /* exact division   */
    return (seq_t)val;
}

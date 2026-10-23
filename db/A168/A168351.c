/*
 * A168351  a(n) = n^5*(n+1)/2.
 * https://oeis.org/A168351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(n+1)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 45, u64 n <= 1824,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit accumulator to avoid overflow for the 64‑bit build. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = nn * nn * nn * nn * nn;   /* n^5 */
    unsigned __int128 val  = prod * (nn + 1) / 2;
    return (seq_t)val;
}

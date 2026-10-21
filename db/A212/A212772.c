/*
 * A212772  a(n) = floor((n+1)*(n-3)*(n-4)/12).
 * https://oeis.org/A212772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(n-3)*(n-4)/12)
 * Range:     u8 n <= 16, u16 n <= 94, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a 128‑bit integer for the intermediate product to avoid
       overflow for the n values used in the tests (up to about 10000). */
    __int128 nn = n;
    __int128 num = (nn + 1) * (nn - 3) * (nn - 4);
    return (seq_t)(num / 12);
}

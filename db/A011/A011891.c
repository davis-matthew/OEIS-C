/*
 * A011891  a(n) = floor(n*(n-1)*(n-2)/9).
 * https://oeis.org/A011891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/9)
 * Range:     u8 n <= 14, u16 n <= 84, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    unsigned __int128 prod = (unsigned __int128)(seq_t)n *
                             (n - 1) *
                             (n - 2);
    prod /= 9;
    return (seq_t)prod;
}

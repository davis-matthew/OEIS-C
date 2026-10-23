/*
 * A014205  a(n) = (1/12)*(n+5)*(n+1)*n^2.
 * https://oeis.org/A014205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)*(n+1)*n^2 / 12, offset 0
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 474, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128-bit arithmetic to avoid overflow before the division */
    unsigned __int128 prod =
        (unsigned __int128)n * n *
        (n + 1ULL) *
        (n + 5ULL);

    prod /= 12U; /* exact division */
    return (seq_t)prod;
}

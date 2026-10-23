/*
 * A172046  Partial sums of floor(n^2/7) (A056834).
 * https://oeis.org/A172046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 11*n + 6)/42) (OEIS A172046)
 * Range:     u8 n <= 17, u16 n <= 110, u32 n <= 4484, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow for 64‑bit seq_t */
    unsigned __int128 N = n;
    unsigned __int128 num = (unsigned __int128)2 * N * N * N
                           + (unsigned __int128)3 * N * N
                           - (unsigned __int128)11 * N
                           + 6;
    return (seq_t)(num / 42);
}

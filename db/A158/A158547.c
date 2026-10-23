/*
 * A158547  a(n) = 24*n^2 + 1.
 * https://oeis.org/A158547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for intermediate computation to avoid overflow
       of the 8‑bit or 16‑bit seq_t while still being correct when the
       result fits in seq_t. */
    unsigned __int128 m = (unsigned __int128)n;
    unsigned __int128 v = 24 * m * m + 1;
    return (seq_t)v;
}

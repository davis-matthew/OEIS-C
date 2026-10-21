/*
 * A054552  a(n) = 4*n^2 - 3*n + 1.
 * https://oeis.org/A054552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^2 - 3*n + 1
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate so that intermediate
       multiplication does not overflow a 64‑bit seq_t. */
    unsigned __int128 u = n;
    unsigned __int128 r = 4 * u * u - 3 * u + 1;
    return (seq_t)r;
}

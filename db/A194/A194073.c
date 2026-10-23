/*
 * A194073  a(n) = 1 + floor((3/4)*n^2).
 * https://oeis.org/A194073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((3/4)*n^2)
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3*n^2 / 4 using a 128‑bit intermediate to avoid overflow
       when seq_t is 64 bits. */
    __int128 t = (__int128)n * n;
    t = t * 3;
    t = t / 4;
    t = t + 1;
    return (seq_t)t;
}

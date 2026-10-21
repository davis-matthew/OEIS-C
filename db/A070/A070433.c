/*
 * A070433  a(n) = n^2 mod 9.
 * https://oeis.org/A070433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 9
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 9 using the 64‑bit integer type to avoid overflow
       when the result type seq_t might be smaller (e.g., uint8_t). */
    idx_t r = n % 9;
    seq_t m = (seq_t)r;
    return (m * m) % 9;
}

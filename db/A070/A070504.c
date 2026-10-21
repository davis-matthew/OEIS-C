/*
 * A070504  a(n) = n^3 mod 42.
 * https://oeis.org/A070504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 42
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 42 first to keep all intermediate values small */
    idx_t m = n % 42;
    idx_t c = (m * m) % 42;   /* m^2 mod 42 */
    c   = (c * m) % 42;       /* m^3 mod 42 */
    return (seq_t)c;
}

/*
 * A070440  a(n) = n^2 mod 18.
 * https://oeis.org/A070440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 18
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the result is always < 18, we can reduce n modulo 18 first
       to avoid overflow on small seq_t types. */
    uint64_t r = n % 18;
    r = (r * r) % 18;
    return (seq_t)r;
}

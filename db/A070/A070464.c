/*
 * A070464  a(n) = n^2 mod 42.
 * https://oeis.org/A070464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 42
 * Range:     u8 n <= 1400, u16 n <= 1400, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 42 first to avoid overflow when seq_t is smaller than idx_t */
    seq_t r = (seq_t)(n % 42);
    return (r * r) % 42;
}

/*
 * A070450  a(n) = n^2 mod 28.
 * https://oeis.org/A070450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 28
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 28 first to avoid overflow on
       models where seq_t is a small integer type. */
    seq_t r = (seq_t)(n % 28ULL);
    return (r * r) % (seq_t)28;
}

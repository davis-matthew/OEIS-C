/*
 * A159852  n^2 mod 60.
 * https://oeis.org/A159852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 60
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 60 first to avoid overflow on large seq_t types. */
    idx_t m = n % 60;
    seq_t res = (seq_t)m * (seq_t)m % 60;
    return res;
}

/*
 * A183033  Minute with hour hand overlap problem on analog clock. Fractions
 *          of seconds.
 * https://oeis.org/A183033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = (3*n) mod 11 (equivalent to floor(300*n/11) mod 12)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce n modulo 11 first to avoid overflow on very large n */
    idx_t r = n % 11;
    return (seq_t)((3 * r) % 11);
}

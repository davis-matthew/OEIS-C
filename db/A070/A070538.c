/*
 * A070538  a(n) = n^4 mod 19.
 * https://oeis.org/A070538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 19
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 19 first to avoid overflow */
    seq_t m = (seq_t)(n % 19);
    /* Compute (m^2) mod 19 */
    seq_t m2 = (m * m) % 19;
    /* Compute (m4) mod 19 = (m2^2) mod 19 */
    seq_t m4 = (m2 * m2) % 19;
    return m4;
}

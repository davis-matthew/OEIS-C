/*
 * A085339  Modulo 91 remainders of 6th powers.
 * https://oeis.org/A085339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = n^6 mod 91
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 91;        /* 0 ≤ r < 91  */
    idx_t r2 = (r * r) % 91; /* r^2 mod 91   */
    idx_t r4 = (r2 * r2) % 91;/* r^4 mod 91   */
    idx_t r6 = (r4 * r2) % 91;/* r^6 mod 91   */
    return (seq_t)r6;
}

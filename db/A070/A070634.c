/*
 * A070634  a(n) = n^6 mod 11.
 * https://oeis.org/A070634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 11
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 11 is enough because of periodicity */
    idx_t r = n % 11;
    seq_t x = (seq_t)r;            /* base, 0 ≤ x ≤ 10 */
    seq_t x2 = (x * x) % 11;       /* x^2 mod 11 */
    seq_t x4 = (x2 * x2) % 11;     /* x^4 mod 11 */
    seq_t x6 = (x4 * x2) % 11;     /* x^6 mod 11 */
    return x6;
}

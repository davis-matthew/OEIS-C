/*
 * A070599  a(n) = n^5 mod 14.
 * https://oeis.org/A070599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 14
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)(n % 14);                 /* n mod 14 */
    seq_t t2 = (x * x) % (seq_t)14;            /* x^2 mod 14 */
    seq_t t4 = (t2 * t2) % (seq_t)14;          /* x^4 mod 14 */
    seq_t res = (x * t4) % (seq_t)14;          /* x^5 mod 14 */
    return res;
}

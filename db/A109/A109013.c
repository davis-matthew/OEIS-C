/*
 * A109013  a(n) = gcd(n,10).
 * https://oeis.org/A109013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = gcd(n,10) with offset 0 (a(0)=10)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n mod 10, then deduce gcd with 10 */
    idx_t r = n % 10;
    switch (r) {
    case 0:          return (seq_t)10;   /* divisible by 10 */
    case 5:          return (seq_t)5;    /* divisible by 5 only */
    case 2: case 4: case 6: case 8:     return (seq_t)2;   /* even but not 10 */
    default:         return (seq_t)1;    /* otherwise coprime with 10 */
    }
}

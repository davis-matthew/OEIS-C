/*
 * A040752  Continued fraction for sqrt(780).
 * https://oeis.org/A040752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040752
**   Continued fraction for sqrt(780).
**   Multiplicative with a(2) = 12, a(2^e) = 54 for e >= 2,
**   a(p^e) = 1 for odd prime p.
**   a(0) = 27.
**/
seq_t a(idx_t n)
{
    /* Handle the offset 0 */
    if (n == 0)
        return (seq_t)27;

    /* n > 0: factor out powers of 2 */
    if ((n & 1) == 0) {           /* even */
        idx_t e = 0;
        while ((n & 1) == 0) {
            e++;
            n >>= 1;              /* remove a factor of 2 */
        }
        return (e == 1) ? (seq_t)12 : (seq_t)54;
    }
    /* odd n: value is 1 */
    return (seq_t)1;
}

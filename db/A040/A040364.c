/*
 * A040364  Continued fraction for sqrt(384).
 * https://oeis.org/A040364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040364: continued fraction terms of sqrt(384)
   a(0) = 19
   For n >= 1: a(n) repeats the pattern [1,1,2,9,2,1,1,38] with period 8
*/
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)19;
    }
    /* pattern for n>=1: a(1)=1, a(2)=1, a(3)=2, a(4)=9,
       a(5)=2, a(6)=1, a(7)=1, a(8)=38 */
    static const seq_t pattern[8] = {1, 1, 2, 9, 2, 1, 1, 38};
    /* n-1 is safe because n>0; modulo 8 is done by bitwise AND with 7 */
    idx_t idx = (n - 1) & 7ULL;
    return pattern[idx];
}

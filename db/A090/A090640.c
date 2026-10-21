/*
 * A090640  a(0) = 0; a(2n) = 4*a(n), a(2n+1) = a(n) + 1.
 * https://oeis.org/A090640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   a(2n)   = 4 * a(n);
   a(2n+1) = a(n) + 1
*/
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    if ((n & 1) == 0) {          /* n is even */
        return (seq_t)4 * a(n >> 1);
    } else {                     /* n is odd and > 0 */
        return a(n >> 1) + 1;
    }
}

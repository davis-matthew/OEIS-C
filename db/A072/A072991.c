/*
 * A072991  Numbers 1 through 10 in alphabetical order.
 * https://oeis.org/A072991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers 1 through 10 in alphabetical order
   a(1)=8, a(2)=5, a(3)=4, a(4)=9, a(5)=1,
   a(6)=7, a(7)=6, a(8)=10, a(9)=3, a(10)=2
*/

seq_t a(idx_t n)
{
    static const seq_t seq[10] = {8, 5, 4, 9, 1, 7, 6, 10, 3, 2};
    if (n >= 1 && n <= 10) {
        return seq[n - 1];
    }
    return 0; /* undefined for n > 10 */
}

/*
 * A152485  Degrees of irreducible representations of SL(2,13).
 * https://oeis.org/A152485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=1; a(2)=a(3)=6; a(4)=a(5)=7;
 *            a(6)=a(7)=a(8)=a(9)=a(10)=a(11)=12; a(12)=13;
 *            a(13)=a(14)=a(15)=a(16)=a(17)=14; a(n)=0 for n>17
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* values before the offset are not defined */
    if (n == 1) return (seq_t)1;
    if (n <= 3) return (seq_t)6;          /* a(2)=a(3)=6 */
    if (n <= 5) return (seq_t)7;          /* a(4)=a(5)=7 */
    if (n <= 11) return (seq_t)12;        /* a(6)..a(11)=12 */
    if (n == 12) return (seq_t)13;        /* a(12)=13 */
    if (n <= 17) return (seq_t)14;        /* a(13)..a(17)=14 */
    return (seq_t)0;                      /* sequence terminates after 17 terms */
}

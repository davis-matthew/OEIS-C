/*
 * A176742  Expansion of (1 - x^2) / (1 + x^2) in powers of x.
 * https://oeis.org/A176742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; a(n)=0 if n is odd; a(n)=2 if n%4==0 and n>0; a(n)=-2 if
 *            n%4==2
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n & 1)   /* odd */
        return (seq_t)0;
    if ((n & 3) == 0)  /* n % 4 == 0 */
        return (seq_t)2;
    return (seq_t)-2;  /* n % 4 == 2 */
}

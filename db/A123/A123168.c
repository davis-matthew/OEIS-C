/*
 * A123168  Continued fraction for sqrt(2)*(exp(sqrt(2))-1)/(exp(sqrt(2))+1).
 * https://oeis.org/A123168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(1)=0; a(n)= (2*n-3)*(3-(-1)^n)/2 for n>1
 * Range:     u8 n <= 66, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)0;
    }
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {          /* even n */
        return sn * (seq_t)2 - (seq_t)3;   /* 2*n-3 */
    } else {                     /* odd n */
        return sn * (seq_t)4 - (seq_t)6;   /* 4*n-6 */
    }
}

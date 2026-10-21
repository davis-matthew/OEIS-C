/*
 * A084099  Expansion of (1+x)^2/(1+x^2).
 * https://oeis.org/A084099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>0, if n is odd then a(n)=2*(-1)^{floor(n/2)},
 *            otherwise a(n)=0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if ((n & 1) == 0)  /* even n > 0 */
        return 0;
    /* odd n: sign depends on floor(n/2) */
    return (n >> 1) & 1 ? (seq_t)-2 : (seq_t)2;
}

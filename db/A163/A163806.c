/*
 * A163806  Expansion of (1 - x^3) * (1 - x^4) / ((1 - x) * (1 - x^6)) in
 *          powers of x.
 * https://oeis.org/A163806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) derived from (1-x³)(1-x⁴)/((1-x)(1-x⁶)). The sequence is
 *            periodic modulo 6 with: a(0) = 1 a(n) = 0 for n>0 divisible by 3
 *            (i.e., n%3==0) a(n) = 1 if n%6 is 1 or 2 a(n) = -1 if n%6 is 4
 *            or 5
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    unsigned r = (unsigned)(n % 6);
    switch (r) {
        case 0:  return (seq_t)0;          /* n>0 and n%6==0 -> multiple of 3 */
        case 1:
        case 2:  return (seq_t)1;
        case 3:  return (seq_t)0;
        case 4:
        case 5:  return (seq_t)(-1);
    }
    return (seq_t)0;   /* unreachable, keeps compiler happy */
}

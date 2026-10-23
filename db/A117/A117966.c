/*
 * A117966  Balanced ternary enumeration (based on balanced ternary
 *          representation) of integers; write n in ternary and then replace
 *          2's with (-1)'s.
 * https://oeis.org/A117966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign, look
 * Formula:   a(0)=0; a(3n)=3*a(n); a(3n+1)=3*a(n)+1; a(3n+2)=3*a(n)-1
 * Range:     i8 n <= 242, i16 n <= 9922, i32 n <= 9922, i64 n <= 9922,
 *            i128 n <= 9922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    idx_t m = n / 3;
    seq_t base = a(m);
    int d = (int)(n % 3);
    if (d == 0) return base * (seq_t)3;
    if (d == 1) return base * (seq_t)3 + (seq_t)1;
    /* d == 2 */
    return base * (seq_t)3 - (seq_t)1;
}

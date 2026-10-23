/*
 * A156174  Period 5: repeat [1,-1,1,-1,0].
 * https://oeis.org/A156174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = pattern with period 5: [1, -1, 1, -1, 0] repeated
 *            Equivalent recurrence: a(n) + a(n-1) + a(n-2) + a(n-3) + a(n-4)
 *            = 0 (n > 4), with initial values a(0)=1, a(1)=-1, a(2)=1,
 *            a(3)=-1, a(4)=0
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static pattern of period 5 */
    static const seq_t pattern[5] = { (seq_t)1, (seq_t)-1, (seq_t)1, (seq_t)-1, (seq_t)0 };
    idx_t r = n % 5;
    return pattern[r];
}

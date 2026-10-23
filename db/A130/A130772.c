/*
 * A130772  Periodic sequence with period 2 2 0 -2 -2 0.
 * https://oeis.org/A130772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=2, a(1)=2, a(n)=a(n-1)-a(n-2) for n>=2
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {
        (seq_t)2, (seq_t)2, (seq_t)0,
        (seq_t)-2, (seq_t)-2, (seq_t)0
    };
    unsigned idx = (unsigned)(n % 6);
    return pat[idx];
}

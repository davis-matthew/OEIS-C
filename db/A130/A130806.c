/*
 * A130806  Period 6: repeat 1,4,3,-1,-4,-3.
 * https://oeis.org/A130806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(0)=1, a(1)=4, a(n)=a(n-1)-a(n-2) for n≥2 (period 6)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = { (seq_t)1, (seq_t)4, (seq_t)3, (seq_t)-1, (seq_t)-4, (seq_t)-3 };
    return period[n % 6];
}

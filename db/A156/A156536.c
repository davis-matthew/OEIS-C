/*
 * A156536  Period 12: repeat 7,5,-1,1,-5,-7,-7,-5,1,-1,5,7.
 * https://oeis.org/A156536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-6), a(0)=7, a(1)=5, a(2)=-1, a(3)=1, a(4)=-5,
 *            a(5)=-7
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for 0 <= n < 6 */
    static const seq_t base[6] = {7, 5, -1, 1, -5, -7};

    /* reduce n by the recurrence a(n) = -a(n-6) */
    idx_t k = n / 6;           /* number of times we subtract 6   */
    idx_t r = n % 6;           /* remainder is the index in base  */
    seq_t val = base[r];

    /* each subtraction of 6 flips the sign;
       if k is odd, negate the value   */
    if (k & 1)
        val = -val;

    return val;
}

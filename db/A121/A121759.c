/*
 * A121759  In decimal number system, take even digits of n with negative
 *          sign.
 * https://oeis.org/A121759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign, nice, look
 * Formula:   a(n) = Σ_{i} sgn(d_i) * d_i * 10^i, where d_i is the i-th
 *            decimal digit of n from the right, and sgn(d_i)=+1 if d_i is
 *            odd, -1 if d_i is even
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t sum   = 0;
    seq_t place = 1;

    while (temp > 0) {
        idx_t d = temp % 10;
        seq_t term = (seq_t)d * place;
        if ((d & 1) == 0)   /* even digit */
            sum -= term;
        else                /* odd digit */
            sum += term;

        temp /= 10;
        if (temp > 0)
            place *= 10;
    }
    return sum;
}

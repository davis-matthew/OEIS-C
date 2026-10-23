/*
 * A131715  Period 12: repeat 0,2,4,0,-6,8,0,-2,-4,0,6,-8.
 * https://oeis.org/A131715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n % 3 == 0; For n % 6 == 1 or 4: magnitude =
 *            (n%6==1)?2:6; Sign = (-1)^{floor(n/6)} * (n%6==4? -1 : 1); For n
 *            % 6 == 2 or 5: magnitude = (n%6==2)?4:8; Sign =
 *            (-1)^{floor(n/6)};
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t norm = n % 6;

    /* Zero for multiples of 3 */
    if (norm == 0 || norm == 3)
        return 0;

    /* parity of floor(n/6) */
    seq_t sign = (((n / 6) & 1) ? (seq_t)(-1) : (seq_t)1);

    if (norm == 1 || norm == 4) {
        seq_t mag = (norm == 1 ? (seq_t)2 : (seq_t)6);
        if (norm == 4)
            sign = -sign;        /* flip sign for n % 12 == 4,10 */
        return mag * sign;
    }

    /* norm == 2 or 5 */
    seq_t mag = (norm == 2 ? (seq_t)4 : (seq_t)8);
    return mag * sign;
}

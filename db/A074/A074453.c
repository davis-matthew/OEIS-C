/*
 * A074453  Sum of determinants of 2nd order principal minors of powers of
 *          inverse of the matrix ((1,1,0,0),(1,0,1,0),(1,0,0,1),(1,0,0,0)).
 * https://oeis.org/A074453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)-2a(n-2)+2a(n-3)+2a(n-4)+a(n-5)+a(n-6) with initial
 *            values a(0)=6, a(1)=1, a(2)=-3, a(3)=1, a(4)=17, a(5)=16
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[6] = {6, 1, -3, 1, 17, 16};

    if (n < 6) {
        return base[n];
    }

    seq_t prev[6];
    for (idx_t i = 0; i < 6; ++i) {
        prev[i] = base[i];
    }

    seq_t cur;
    for (idx_t i = 6; i <= n; ++i) {
        cur = prev[5] - (seq_t)2 * prev[4]
              + (seq_t)2 * prev[3] + (seq_t)2 * prev[2]
              + prev[1] + prev[0];

        /* shift previous values */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }

    return cur;
}

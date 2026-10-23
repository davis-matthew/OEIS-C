/*
 * A122173  Expansion of -x * (x^5+x^4-15*x^3+19*x^2-8*x+1) /
 *          (x^6-12*x^5+34*x^4-30*x^3+6*x^2+3*x-1).
 * https://oeis.org/A122173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 38, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Linear recurrence for OEIS A122173:
   a(n) = 3*a(n-1)+6*a(n-2)-30*a(n-3)+34*a(n-4)-12*a(n-5)+a(n-6)
   Initial values (offset 1): a(1)=1, a(2)=-5, a(3)=10, a(4)=-45, a(5)=110, a(6)=-421
*/
seq_t a(idx_t n)
{
    /* Pre‑computed first six values */
    static const seq_t init[6] = {  (seq_t)1,
                                    (seq_t)-5,
                                    (seq_t)10,
                                   (seq_t)-45,
                                    (seq_t)110,
                                   (seq_t)-421 };

    if (n <= 6)
        return init[n - 1];

    /* Move a(n-6)...a(n-1) through a ring buffer */
    seq_t last[6];
    for (idx_t i = 0; i < 6; ++i)
        last[i] = init[i];

    seq_t cur = 0;
    for (idx_t k = 7; k <= n; ++k) {
        /* a(k) = 3*a(k-1)+6*a(k-2)-30*a(k-3)+34*a(k-4)-12*a(k-5)+a(k-6) */
        cur =  3 * last[5]
              + 6 * last[4]
             -30 * last[3]
             +34 * last[2]
             -12 * last[1]
             + 1 * last[0];

        /* shift buffer: drop a(k-6) and push a(k) */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = cur;
    }
    return cur;
}

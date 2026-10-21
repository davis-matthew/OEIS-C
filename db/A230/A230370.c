/*
 * A230370  Voids left after packing 3 curves coins patterns (3c3s type) into
 *          fountain of coins base n.
 * https://oeis.org/A230370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence * a(1)=0, a(2)=0, a(3)=3,
 *            a(4)=6, a(5)=13, ... * a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) -
 *            a(n-4) + a(n-5) + p(n) * where p(n) is the coefficient of x^n in
 *            * P(x)=x^3*(11*x^8 - 5*x^7 - 21*x^6 + 6*x^5 + 9*x^4 + x^2 + 3*x
 *            + 3) * giving * p(3)=3, p(4)=3, p(5)=1, p(6)=0...
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* coefficients p(n) for 0 ≤ n ≤ 11, 0 else */
    static const int p[12] = {
        0, 0, 0,        /* p(0),p(1),p(2) */
        3, 3, 1,        /* p(3)..p(5) */
        0, 9, 6,        /* p(6)..p(8) */
       -21, -5, 11      /* p(9)..p(11) */
    };

    seq_t a0 = (seq_t)0;  /* a(n-1)  */
    seq_t a1 = (seq_t)0;  /* a(n-2)  */
    seq_t a2 = (seq_t)0;  /* a(n-3)  */
    seq_t a3 = (seq_t)0;  /* a(n-4)  */
    seq_t a4 = (seq_t)0;  /* a(n-5)  */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute base = a0 + 2*a1 + a4 */
        seq_t base = a0 + ((seq_t)2 * a1) + a4;
        /* compute diff = 2*a2 + a3 */
        seq_t diff = ((seq_t)2 * a2) + a3;
        /* ak = base - diff + p(k) */
        seq_t ak = base - diff;
        if (k < 12) {
            int pk = p[k];
            if (pk > 0)
                ak += (seq_t)pk;
            else if (pk < 0)
                ak -= (seq_t)(-pk);
        }
        /* shift the window for next iteration */
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = a0;
        a0 = ak;
    }
    return a0;
}

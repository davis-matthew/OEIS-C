/*
 * A193006  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-5*a(n-2)+a(n-3)+2*a(n-4)-a(n-5) for n≥5 initial
 *            values: a(0)=1, a(1)=0, a(2)=8, a(3)=27, a(4)=72
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First five values of the sequence */
    static const seq_t init[5] = { 1, 0, 8, 27, 72 };

    /* For indices below 5 just return the pre‑computed value */
    if (n < 5)
        return init[n];

    /* Circular buffer holding the last five values:
       buf[0] = a(k-1), buf[1] = a(k-2), buf[2] = a(k-3),
       buf[3] = a(k-4), buf[4] = a(k-5)   */
    seq_t buf[5] = { init[4], init[3], init[2], init[1], init[0] };

    for (idx_t k = 5; k <= n; ++k) {
        seq_t left  = 4 * buf[0] + buf[2] + (seq_t)2 * buf[3];  /* 4*a(k-1)+a(k-3)+2*a(k-4) */
        seq_t right = 5 * buf[1] + buf[4];                       /* 5*a(k-2)+a(k-5) */
        seq_t next  = left - right;                              /* a(k) */

        /* Shift buffer for next iteration */
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0];
}

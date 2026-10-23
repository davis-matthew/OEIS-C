/*
 * A041583  Denominators of continued fraction convergents to sqrt(309).
 * https://oeis.org/A041583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 25, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 128405450990*a(n-26) - a(n-52) for n>51,
   computed via continued fraction for √309.
   Initial values a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=19,
   … follow from the period length‑26 expansion of √309.  */
seq_t a(idx_t n)
{
    /* Period of the continued fraction for √309: [1,1,2,1,2,4,1,1,1,8,6,1,
       10,1,6,8,1,1,1,4,2,1,2,1,1,34] */
    static const seq_t period[26] = {
        1, 1, 2, 1, 2, 4, 1, 1, 1, 8, 6, 1,
      10, 1, 6, 8, 1, 1, 1, 4, 2, 1, 2, 1, 1, 34
    };
    /* Integer part of √309 */
    static const seq_t a0 = 17;

    /* Recurrence for convergents:
       p_-2 = 0, p_-1 = 1
       q_-2 = 1, q_-1 = 0   */
    seq_t p_prev  = 1, p_prev2 = 0;   /* p_{k-1}, p_{k-2}   */
    seq_t q_prev  = 0, q_prev2 = 1;   /* q_{k-1}, q_{k-2}   */
    seq_t q = 0;                      /* the value to return */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = (k == 0) ? a0 :
                   period[(unsigned)((k - 1) % 26u)];
        /* p_k = a_k * p_{k-1} + p_{k-2}   */
        seq_t p = ak * p_prev + p_prev2;
        /* q_k = a_k * q_{k-1} + q_{k-2}   */
        q = ak * q_prev + q_prev2;
        /* shift for next iteration */
        p_prev2 = p_prev;
        p_prev  = p;
        q_prev2 = q_prev;
        q_prev  = q;
    }
    return q;
}

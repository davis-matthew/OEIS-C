/*
 * A042717  Denominators of continued fraction convergents to sqrt(888).
 * https://oeis.org/A042717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=c[n]*a(n-1)+a(n-2);  c[n] cycles 1,3,1,58 (for n>=1) */
seq_t a(idx_t n)
{
    /* Periodic coefficients of the continued fraction of sqrt(888) */
    static const seq_t period[4] = {1, 3, 1, 58};

    /* Handle the base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t prev_prev = 0;   /* q_{-1} */
    seq_t prev      = 1;   /* q_0 */
    seq_t current;

    idx_t i;
    for (i = 1; i <= n; ++i)
    {
        seq_t coef = period[(i - 1) & 3];
        current = coef * prev + prev_prev;
        prev_prev = prev;
        prev = current;
    }

    return prev;
}

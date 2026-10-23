/*
 * A042194  Numerators of continued fraction convergents to sqrt(622).
 * https://oeis.org/A042194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042194: Numerators of continued‑fraction convergents to sqrt(622).
   The recurrence is          p(-2)=0, p(-1)=1,
   p(n) = a(n)*p(n-1)+p(n-2),
   with a(0)=24 and for n≥1 the terms a(n) follow the periodic pattern
   [1, 15, 1, 1, 1, 4, 1, 7, 2, 24, 2, 7, 1, 4, 1, 1, 1, 15, 1, 48] (period length 20). */

seq_t a(idx_t n)
{
    static const int period[20] =
        {1, 15, 1, 1, 1, 4, 1, 7, 2, 24,
         2, 7, 1, 4, 1, 1, 1, 15, 1, 48};

    seq_t p_prev_prev = 0;          /* p(-2) */
    seq_t p_prev       = 1;          /* p(-1) */
    seq_t p_cur        = 0;          /* current numerator */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)24;
        } else {
            a_k = (seq_t)period[(k - 1) % 20];
        }

        if (k == 0) {
            p_cur = a_k;                           /* p0 */
        } else if (k == 1) {
            p_cur = a_k * p_prev + (seq_t)1;        /* p1 uses p(-1)=1 */
        } else {
            p_cur = a_k * p_prev + p_prev_prev;     /* general recurrence */
        }

        if (k == n)
            return p_cur;

        /* shift for the next iteration */
        p_prev_prev = p_prev;
        p_prev      = p_cur;
    }

    return (seq_t)0; /* unreachable */
}

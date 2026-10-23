/*
 * A041963  Denominators of continued fraction convergents to sqrt(504).
 * https://oeis.org/A041963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(504).
   Recurrence: a(0)=1, a(-1)=0,
   a(n) = coeff(n) * a(n-1) + a(n-2)
   where coeff(n) is periodic: [2, 4, 2, 44] repeated. */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(504) */
    static const idx_t period[4] = {2, 4, 2, 44};
    const idx_t period_len = 4;

    /* Base cases: a(-1)=0, a(0)=1 */
    seq_t q_prev = 0;   /* a(-1) */
    seq_t q_curr = 1;   /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = (seq_t)period[(i - 1) % period_len];
        seq_t next_q = coeff * q_curr + q_prev;
        q_prev = q_curr;
        q_curr = next_q;
    }
    return q_curr;
}

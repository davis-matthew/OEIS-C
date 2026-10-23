/*
 * A042355  Denominators of continued fraction convergents to sqrt(704).
 * https://oeis.org/A042355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042355: denominators of continued fraction convergents to sqrt(704)
   Recurrence:  q(0)=1, q(1)=1,
                q(n)=a(n)*q(n-1)+q(n-2)  for n≥2
   where a(n) follows the periodic list 1,1,7,13,7,1,1,52 (period 8). */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t q_prevprev = 0;
    seq_t q_prev = 1;
    static const seq_t period[8] = {1,1,7,13,7,1,1,52};
    const idx_t period_len = 8;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_val = period[(k - 1) % period_len];
        seq_t q_curr = a_val * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;
}

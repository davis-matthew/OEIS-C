/*
 * A041091  Denominators of continued fraction convergents to sqrt(53).
 * https://oeis.org/A041091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents of the continued fraction of sqrt(53).
   Recurrence: q_{-2}=1, q_{-1}=0, and for k≥0:
   q_k = a_k * q_{k-1} + q_{k-2},
   where a_0 = 7 and for k≥1 the partial quotients repeat
   3, 1, 1, 3, 14 (period 5).  Thus a(n) = q_n. */
seq_t a(idx_t n)
{
    static const int period[5] = {3, 1, 1, 3, 14};
    seq_t q_prevprev = 1;    /* q_{-2} */
    seq_t q_prev    = 0;     /* q_{-1} */
    for (idx_t k = 0; k <= n; ++k)
    {
        int ak = (k == 0) ? 7 : period[(k - 1) % 5];
        seq_t q_curr = (seq_t)ak * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;
}

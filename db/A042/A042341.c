/*
 * A042341  Denominators of continued fraction convergents to sqrt(697).
 * https://oeis.org/A042341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042341: Denominators of continued fraction convergents to sqrt(697)
 *  Computed by recurrence with periodic partial quotients:
 *      a0 = 26
 *      a_k for k ≥ 1:  2, 2, 52, 2, 2, 52, ...
 *  Denominator recurrence:
 *      q_-2 = 1,  q_-1 = 0
 *      q_k = a_k * q_{k-1} + q_{k-2}
 *  The sequence a(n) is q_n.
 */
seq_t a(idx_t n)
{
    /* q_{k-2} and q_{k-1} initialized so that q_0 = 1, q_1 = 2, etc. */
    seq_t q_prevprev = (seq_t)1;  /* q_{-2} */
    seq_t q_prev     = (seq_t)0;  /* q_{-1} */

    seq_t ai;
    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            ai = (seq_t)26;
        } else if (k % 3 == 0) {
            ai = (seq_t)52;
        } else {
            ai = (seq_t)2;
        }

        seq_t q_curr = ai * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;   /* q_n is the required denominator */
}

/*
 * A041955  Denominators of continued fraction convergents to sqrt(500).
 * https://oeis.org/A041955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the nth convergent of sqrt(500).
 *            Partial quotients: a0 = 22; then a1.. =
 *            2,1,3,2,1,1,10,1,1,2,3,1,2,44 repeating. Recurrence for the
 *            denominators q: q(-2) = 1, q(-1) = 0 q_k = a_k * q_(k-1) +
 *            q_(k-2) (k ≥ 0) Then a(n) = q_n (n = 0 gives q_0 = 1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 23, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char _cf_square500_partial[14] =
{ 2, 1, 3, 2, 1, 1, 10, 1, 1, 2, 3, 1, 2, 44 };

seq_t a(idx_t n)
{
    seq_t q_prevprev = (seq_t)1;   /* q(-2) */
    seq_t q_prev = (seq_t)0;       /* q(-1) */
    seq_t q_cur = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        unsigned a_k;
        if (k == 0)
            a_k = 22;
        else
            a_k = _cf_square500_partial[(k - 1) % 14U];

        /* Using seq_t arithmetic; cast a_k to seq_t */
        q_cur = (seq_t)a_k * q_prev + q_prevprev;

        /* Shift for next iteration */
        q_prevprev = q_prev;
        q_prev = q_cur;
    }

    return q_cur;
}

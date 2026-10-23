/*
 * A042377  Denominators of continued fraction convergents to sqrt(715).
 * https://oeis.org/A042377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of convergents to sqrt(715)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sqrt(715) has integral part 26 and a continued‑fraction period of length 10:
       [1, 2, 1, 5, 5, 5, 1, 2, 1, 52] */
    static const unsigned short period[10] =
        {1, 2, 1, 5, 5, 5, 1, 2, 1, 52};

    /* Convergent recurrence: p_{-2}=0, p_{-1}=1; q_{-2}=1, q_{-1}=0 */
    seq_t p_nm2 = 0, p_nm1 = 1, q_nm2 = 1, q_nm1 = 0;
    seq_t pk = 0, qk = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t ak = (k == 0) ? (seq_t)26
                            : (seq_t)period[(k - 1) % 10];
        pk = ak * p_nm1 + p_nm2;
        qk = ak * q_nm1 + q_nm2;
        /* shift variables for next iteration */
        p_nm2 = p_nm1;  p_nm1 = pk;
        q_nm2 = q_nm1;  q_nm1 = qk;
    }
    return qk;
}

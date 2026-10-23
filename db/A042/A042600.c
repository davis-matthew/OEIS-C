/*
 * A042600  Numerators of continued fraction convergents to sqrt(829).
 * https://oeis.org/A042600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(829)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A042600 (numerators of convergents
 * to sqrt(829)).  The routine uses the standard recurrence for
 * convergents of a quadratic irrational:
 *
 *   p_{-2} = 0,  p_{-1} = 1
 *   a0     = floor(sqrt(829)) = 28
 *   For k >= 0:
 *     p_k = a_k * p_{k-1} + p_{k-2}
 *
 *   a_k are the continued‑fraction coefficients of sqrt(829), generated
 *   by the usual recurrence:
 *     m_{k+1} = d_k * a_k - m_k
 *     d_{k+1} = (829 - m_{k+1}^2) / d_k
 *     a_{k+1} = (a0 + m_{k+1}) / d_{k+1}
 *
 *   The function returns p_n for the requested n (0‑based indexing).
 */
seq_t a(idx_t n)
{
    const seq_t S        = 829;          /* D in sqrt(D) */
    const seq_t a0       = 28;           /* floor(sqrt(829)) */
    seq_t m = 0, d = 1, ak = a0;
    seq_t pkm2 = 0, pkm1 = 1, pk = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0)
        {   /* a0 case */
            pk = ak * pkm1 + pkm2;      /* = 28*1+0 = 28 */
        }
        else
        {   /* advance continued‑fraction coefficients */
            m  = d * ak - m;
            d  = (S - m * m) / d;      /* exact division */
            ak = (a0 + m) / d;
            pk = ak * pkm1 + pkm2;
        }

        if (k == n)
            return pk;

        /* shift for next iteration */
        pkm2 = pkm1;
        pkm1 = pk;
    }

    /* Should never reach here */
    return 0;
}

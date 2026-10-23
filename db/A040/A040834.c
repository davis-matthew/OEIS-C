/*
 * A040834  Continued fraction for sqrt(864).
 * https://oeis.org/A040834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(864)
   a(0) = floor(sqrt(864)) = 29
   For n > 0: a(n) is the (n‑1)th element of the repeating period
   (2,1,1,5,1,13,1,5,1,1,2,58).  */
/* Ensure that the period is computed only once. */
static const idx_t a0          = 29;              /* floor(sqrt(864)) */
static seq_t  period[64];                       /* will hold the period */
static idx_t  period_len = 0;                    /* number of elements in the period */

/* One‐time initialization of the continued fraction period. */
static void init_period(void)
{
    /* If already initialized, nothing to do. */
    if (period_len != 0)
        return;

    idx_t m   = 0;                              /* (m_0) */
    idx_t d   = 1;                              /* (d_0) */
    idx_t a   = a0;                             /* (a_0) */
    const idx_t D = 864;                        /* value under the square root */

    while (1)
    {
        idx_t m_next = d * a - m;               /* m_{k+1} = d_k*a_k - m_k */
        idx_t d_next = (D - m_next * m_next) / d; /* d_{k+1} = (D - m_{k+1}^2)/d_k */
        idx_t a_next = (a0 + m_next) / d_next;     /* a_{k+1} = floor((a0 + m_{k+1})/d_{k+1}) */

        period[period_len++] = (seq_t)a_next;   /* store the term */

        /* The period terminates when the next term equals 2*a0. */
        if (a_next == 2 * a0)
            break;

        /* Prepare for next iteration. */
        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/* Return the nth term of the sequence (OEIS index: a(0)). */
seq_t a(idx_t n)
{
    init_period();                                 /* ensures period ready */

    if (n == 0)
        return (seq_t)a0;                          /* a(0) */

    /* For n > 0, use the periodic part. */
    return period[(n - 1) % period_len];
}

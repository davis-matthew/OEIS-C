/*
 * A064887  Period of continued fraction for sqrt(14)*n.
 * https://oeis.org/A064887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of continued fraction for sqrt(14)*n (OEIS A064887).
   The algorithm computes the classical continued‑fraction
   iteration for sqrt(D) with D = 14·n² and returns the
   number of terms before the period repeats. */

/* Integer square root for an unsigned seq_t value. */
static inline seq_t isqrt_seq(seq_t n)
{
    seq_t lo = 0, hi = n;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq = mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Compute D = 14*n^2 */
    seq_t N = (seq_t)n;
    seq_t D = N * N * (seq_t)14;

    seq_t a0 = isqrt_seq(D);          /* floor(sqrt(D)) */
    seq_t two_a0 = a0 + a0;           /* 2*a0 for period termination */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_curr = a0;
    seq_t period = 0;

    while (1) {
        /* m_k = d_{k-1} * a_{k-1} - m_{k-1} */
        m = d * a_curr - m;

        /* d_k = (D - m_k^2) / d_{k-1}   (guaranteed integer) */
        seq_t m_sq = m * m;
        d = (D - m_sq) / d;

        /* a_k = (a0 + m_k) / d_k   (guaranteed integer) */
        a_curr = (a0 + m) / d;

        period++;

        /* Period ends when a_k == 2*a0 */
        if (a_curr == two_a0)
            break;
    }

    return period;
}

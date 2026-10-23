/*
 * A049990  a(n) is the number of arithmetic progressions of 2 or more
 *          positive integers, nondecreasing with sum n.
 * https://oeis.org/A049990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts non‑decreasing arithmetic progressions
   of positive integers of length ≥2 whose terms sum to n. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t N = (seq_t)n;                 /* use seq_t for arithmetic */

    if (n < 2) return 0;                /* no progression for n = 1 */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t K = (seq_t)k;
        /* M = k*(k-1)/2  computed without overflow */
        seq_t M;
        if (K % 2 == 0)
            M = (K / 2) * (K - 1);
        else
            M = K * ((K - 1) / 2);

        seq_t diff = N - K;            /* n - k  (non‑negative) */
        seq_t dmax = diff / M;         /* floor((n-k)/M) */

        seq_t temp = N;                /* N - d*M will be maintained */
        for (idx_t d = 0; d <= (idx_t)dmax; ++d)
        {
            if (temp % K == 0)
                ++result;

            /* prepare for next d; safe because M*d never exceeds N */
            temp -= M;
        }
    }
    return result;
}

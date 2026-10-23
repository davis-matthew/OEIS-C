/*
 * A177326  Number of permutations of n copies of 1..5 with all adjacent
 *          differences <= 3 in absolute value.
 * https://oeis.org/A177326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(3n, n) * C(2n, n) * Σ_{l=1}^{n} C(3n+1, l) * C(n-1,
 *            l-1) * C(4n-l, n) This counts the permutations of n copies of
 *            1..5 where 1 and 5 are never adjacent. The summation is O(n) and
 *            the entire function runs in linear time
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* a(0) = 1 */

    /* ---------- base = C(3n, n) * C(2n, n) ---------- */
    seq_t C3n_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        C3n_n = C3n_n * (seq_t)(3 * n - i + 1) / i;   /* C(3n, i) */

    seq_t C2n_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        C2n_n = C2n_n * (seq_t)(2 * n - i + 1) / i;   /* C(2n, i) */

    seq_t base = C3n_n * C2n_n;

    /* ---------- summation part ---------- */
    /* current binomial C(n-1, l-1), start with l = 1 */
    seq_t cnm_cur = (seq_t)1;   /* C(n-1, 0) */

    /* current N for C(4n - l, n); start with l = 1, N = 4n - 1 */
    idx_t N_cur = 4 * n - 1;

    /* compute C(N_cur, n) for initial l = 1 */
    seq_t C4_cur = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        C4_cur = C4_cur * (seq_t)(N_cur - n + i) / i;  /* C(4n-1, n) */

    seq_t C3_cur = (seq_t)1;  /* C(3n+1, 0) */
    seq_t sumS   = (seq_t)0;

    for (idx_t l = 1; l <= n; ++l)
    {
        /* C(3n+1, l) */
        C3_cur = C3_cur * (seq_t)(3 * n + 2 - l) / l;

        /* add term to sum */
        sumS += C3_cur * cnm_cur * C4_cur;

        /* prepare values for next l */
        if (l < n)
        {
            /* update C(n-1, l) from C(n-1, l-1) */
            cnm_cur = cnm_cur * (seq_t)(n - l) / l;    /* (n-1 choose l) */

            /* update C(4n - (l+1), n) from C(4n - l, n) */
            C4_cur = C4_cur * (seq_t)(N_cur - n) / N_cur;   /* ensure exact division */
            --N_cur;
        }
    }

    return base * sumS;
}

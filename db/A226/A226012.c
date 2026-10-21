/*
 * A226012  Number of unimodal functions f:[n]->[2n].
 * https://oeis.org/A226012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A226012: Number of unimodal functions f:[n]->[2n].
   Formula: a(n) = Σ_{k=1..n} Σ_{v=1..2n} C(v+k-3, k-1) * C(v+n-k-1, n-k)
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    seq_t total = 0;

    for (idx_t k = 1; k <= n; ++k)          /* peak position */
    {
        idx_t t = n - k;                    /* n-k (right part length) */
        seq_t sum_k  = 0;

        /* v = 1 */
        if (k == 1)                        /* left part exists only for k=1 */
            sum_k += 1;                    /* C(0,0)*C(n-k, n-k) = 1 */

        /* values for v = 2 */
        seq_t A = (k == 1) ? 1 : 1;         /* C(k-1, k-1) = 1 for k>1 */
        seq_t B = (t == 0) ? 1 : (seq_t)(t + 1);  /* C(t+1, t) = t+1 for t>0 */
        sum_k += A * B;                     /* contribution of v = 2 */

        /* iterate v = 3 .. 2n */
        for (idx_t v = 2; v < 2 * n; ++v)   /* v currently corresponds to v+1 in the formula */
        {
            if (k > 1)                      /* update A: C(v+k-2, k-1) from C(v+k-3, k-1) */
            {
                seq_t num = (seq_t)(v + k - 2);   /* (v + k - 2)   */
                seq_t den = (seq_t)(v - 1);       /* (v - 1)       */
                A = (A * num) / den;
            }
            /* else A stays 1 for k = 1 */

            if (t > 0)                      /* update B: C(v+t, t) from C(v+t-1, t) */
            {
                seq_t num = (seq_t)(v + t);     /* (v + t)     */
                seq_t den = (seq_t)v;           /* v           */
                B = (B * num) / den;
            }
            /* else B stays 1 for t = 0 */

            sum_k += A * B;
        }

        total += sum_k;
    }

    return total;
}

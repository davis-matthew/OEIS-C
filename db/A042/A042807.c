/*
 * A042807  Denominators of continued fraction convergents to sqrt(934).
 * https://oeis.org/A042807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6069130*a(n-14) - a(n-28) (for n > 27)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed from the continued‑fraction expansion of sqrt(934). */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Constants for D = 934 */
    const idx_t D   = 934;
    const idx_t A0  = 30;     /* floor(sqrt(934)) */

    /* Initial state of the continued‑fraction algorithm */
    idx_t  m = 0;
    idx_t  d = 1;
    idx_t  a_k = A0;

    /* Initial convergent denominators: q_{-2}=1, q_{-1}=0 */
    seq_t prev2 = 1;          /* q_{-2} */
    seq_t prev1 = 0;          /* q_{-1} */

    seq_t q = 0;              /* current denominator */

    /* Iterate until the requested index n */
    for (idx_t i = 0; i <= n; ++i)
    {
        /* q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t ak_seq = (seq_t)a_k;   /* promote to seq_t */
        q = ak_seq * prev1 + prev2;

        if (i == n)
            return q;            /* result found */

        /* shift for next step */
        prev2 = prev1;
        prev1 = q;

        /* update continued‑fraction parameters for next coefficient */
        m = d * a_k - m;
        d = (D - m * m) / d;
        a_k = (A0 + m) / d;
    }

    return q;  /* unreachable but keeps compiler happy */
}

/*
 * A042177  Denominators of continued fraction convergents to sqrt(613).
 * https://oeis.org/A042177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of sqrt(613) * (OEIS
 *            A042177). The recurrence for the convergents of the * continued
 *            fraction of √613 is * p_{-1}=1, p_0=a0, q_{-1}=0, q_0=1 * a0 =
 *            ⌊√613⌋ = 24 * For k ≥ 0: * m_{k+1} = d_k·a_k – m_k * d_{k+1} =
 *            (613 – m_{k+1}²) / d_k * a_{k+1} = ⌊(a0 + m_...
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *q_cache      = NULL;   /* array of denominators          */
    static size_t   cache_cap  = 0;      /* allocated capacity              */
    static idx_t    max_n      = (idx_t)-1;   /* last index computed  */

    /* continued‑fraction state (valid at last computed index) */
    static seq_t   last_m      = 0;   /* m_k   */
    static seq_t   last_d      = 1;   /* d_k   */
    static seq_t   last_a      = 0;   /* a_k   (current partial quotient) */
    static seq_t   q_prev      = 0;   /* q_{k-1} */
    static seq_t   q_curr      = 1;   /* q_k     */

    /* constants */
    const seq_t D   = 613;
    const seq_t A0  = 24;   /* a0 = floor(sqrt(613)) */

    /* first call: initialise storage and first term */
    if (q_cache == NULL) {
        cache_cap = 1;
        q_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!q_cache) return (seq_t)0;      /* out of memory */

        /* initialise continued‑fraction variables */
        last_a = A0;          /* a0 */
        last_m = (seq_t)0;    /* m_0 = 0                */
        last_d = (seq_t)1;    /* d_0 = 1                */

        /* initial convergents */
        q_prev = (seq_t)0;    /* q_{-1} */
        q_curr = (seq_t)1;    /* q_0   = 1 */

        q_cache[0] = q_curr;  /* a(0) = 1 */
        max_n     = 0;
    }

    /* grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(q_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        q_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to index n (if not already computed) */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t m_next = last_d * last_a - last_m;
        seq_t d_next = (D - m_next * m_next) / last_d;
        seq_t a_next = (A0 + m_next) / d_next;  /* floor division */

        seq_t q_next = a_next * q_curr + q_prev;

        q_cache[i] = q_next;

        /* shift state for next iteration */
        q_prev = q_curr;
        q_curr = q_next;
        last_m  = m_next;
        last_d  = d_next;
        last_a  = a_next;
    }

    max_n = n;
    return q_cache[n];
}

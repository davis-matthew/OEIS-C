/*
 * A198262  Logarithmic derivative of the q-exponential of x, E_q(x,q),
 *          evaluated at q=-x.
 * https://oeis.org/A198262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n+1)*[x^(n+1)] log(∏_{k≥1}(1-(1+x)(-x)^k))
 * Range:     i8 n <= 16, i16 n <= 36, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using recursions: let P(x)=∏_{k=1}^{N} (1-(1+x)(-x)^k)=∑_{m≥0} p_m x^m   */
/* Then s_m=(m)*l_m where l_m=[x^m]log(P(x)) satisfies   s_m = m*p_m - Σ_{t=1}^{m-1} s_t * p_{m-t} */
/* a(n) = s_{n+1} */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static idx_t cache_N = 0;         /* largest m = computed_n+1 processed */
    static seq_t *s_cache = NULL;     /* array of s[m], size cache_N+1 */

    if (n + 1 <= cache_N)
        return s_cache[n + 1];

    /* need to recompute up to m = n+1 */
    idx_t N = n + 1;          /* maximum degree needed in polynomial P(x) */

    /* allocate arrays for polynomial coefficients */
    seq_t *p = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *tmp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= N; ++i)
        p[i] = 0;
    p[0] = 1;   /* P(0) = 1 */

    /* multiply successive factors (1 - (-1)^k x^k - (-1)^k x^{k+1}) */
    for (idx_t k = 1; k <= N; ++k)
    {
        seq_t a_coeff = (k & 1) ? -1 : 1;   /* (-1)^k */
        for (idx_t m = 0; m <= N; ++m)
        {
            seq_t val = p[m];
            if (m >= k)
                val -= a_coeff * p[m - k];
            if (m >= k + 1)
                val -= a_coeff * p[m - k - 1];
            tmp[m] = val;
        }
        /* swap p and tmp */
        seq_t *swap = p;
        p = tmp;
        tmp = swap;
    }
    /* at this point 'p' holds coefficients p[0..N] of P(x) */

    /* free temporary array 'tmp' (now holds old p values) */
    free(tmp);

    /* compute s[1..N] using recurrence s_m = m*p_m - Σ_{t=1}^{m-1} s_t * p_{m-t} */
    seq_t *s = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    s[0] = 0;
    for (idx_t m = 1; m <= N; ++m)
    {
        seq_t tot = (seq_t)m * p[m];
        for (idx_t t = 1; t < m; ++t)
            tot -= s[t] * p[m - t];
        s[m] = tot;
    }

    /* free polynomial coefficient array */
    free(p);

    /* replace cached array */
    if (s_cache)
        free(s_cache);
    s_cache = s;
    cache_N = N;

    return s[n + 1];
}

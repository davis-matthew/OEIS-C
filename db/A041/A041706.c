/*
 * A041706  Numerators of continued fraction convergents to sqrt(373).
 * https://oeis.org/A041706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(373)
 *            Recurrence: p_{-2} = 0, p_{-1} = 1 a_0 = ⌊√373⌋ = 19 For k ≥ 0,
 *            a_{k+1} = ⌊(a_0 + m_k)/d_k⌋, m_{k+1} = d_k * a_k – m_k, d_{k+1}
 *            = (373 – m_{k+1}^2) / d_k, p_k = a_k * p_{k-1} + p_{k-2} (with
 *            p_0 = a_0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms and continued‑fraction state */
    static seq_t  *p_cache = NULL;   /* numerators p_k */
    static idx_t  *a_cache = NULL;   /* partial quotients a_k */
    static idx_t  *m_cache = NULL;   /* m_k values */
    static idx_t  *d_cache = NULL;   /* d_k values */
    static idx_t   cache_len = 0;     /* number of terms stored (≥0) */
    static idx_t   cache_cap = 0;     /* allocated capacity for arrays */

    const idx_t D     = 373;          /* D = 373 */
    const idx_t a0int = 19;           /* floor(sqrt(373)) */

    /* If the requested term is already cached, return it directly. */
    if (n < cache_len)
        return p_cache[n];

    /* Ensure allocated arrays can hold up to n (inclusive). */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;

        p_cache = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        a_cache = (idx_t *)realloc(a_cache, new_cap * sizeof(idx_t));
        m_cache = (idx_t *)realloc(m_cache, new_cap * sizeof(idx_t));
        d_cache = (idx_t *)realloc(d_cache, new_cap * sizeof(idx_t));
        cache_cap = new_cap;
    }

    /* Initialize the first term if we have not done so yet. */
    if (cache_len == 0) {
        m_cache[0] = 0;
        d_cache[0] = 1;
        a_cache[0] = a0int;
        p_cache[0] = (seq_t)a0int;     /* p_0 = a_0 */
        cache_len = 1;
    }

    /* Extend the cache up to the requested index n. */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k > 0) {   /* compute a_k, m_k, d_k for k ≥ 1 */
            idx_t m_prev = m_cache[k - 1];
            idx_t d_prev = d_cache[k - 1];
            idx_t a_prev = a_cache[k - 1];

            idx_t m_curr = d_prev * a_prev - m_prev;
            idx_t d_curr = (D - m_curr * m_curr) / d_prev;
            idx_t a_curr = (a0int + m_curr) / d_curr;

            m_cache[k] = m_curr;
            d_cache[k] = d_curr;
            a_cache[k] = a_curr;
        }

        /* Compute the numerator p_k */
        if (k == 0) {
            /* already set */
        } else if (k == 1) {
            seq_t a1 = (seq_t)a_cache[1];
            seq_t p0 = p_cache[0];
            p_cache[1] = a1 * p0 + (seq_t)1;
        } else {
            seq_t ak   = (seq_t)a_cache[k];
            seq_t pk1  = p_cache[k - 1];
            seq_t pk2  = p_cache[k - 2];
            p_cache[k] = ak * pk1 + pk2;
        }
    }

    cache_len = n + 1;
    return p_cache[n];
}

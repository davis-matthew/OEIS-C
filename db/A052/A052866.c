/*
 * A052866  Expansion of e.g.f. x/(1 - x) + exp(x/(1 - x)).
 * https://oeis.org/A052866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence given by OEIS A052866 a(0)=1, a(1)=2, a(2)=5 For n ≥
 *            0: a(n+3) = (n^3+3n^2+2n)·a(n) - (3n^2+10n+8)·a(n+1) +
 *            (3n+7)·a(n+2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_index = (idx_t)-1;

    /* ensure the cache is large enough */
    if (cache_cap <= n + 3) {
        size_t new_cap = (n + 4) > 8 ? (n + 4) : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize base values if not already done */
    if (last_index == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        last_index = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        /* recurrence uses a(i-3), a(i-2), a(i-1) */
        if (i < 3) continue;          /* safety, base cases are handled above */
        idx_t m = i - 3;
        seq_t m_seq = (seq_t)m;

        /* polynomial coefficients */
        seq_t coeff1 = m_seq * m_seq * m_seq
                     + (seq_t)3 * m_seq * m_seq
                     + (seq_t)2 * m_seq;
        seq_t coeff2 = (seq_t)3 * m_seq * m_seq
                     + (seq_t)10 * m_seq
                     + (seq_t)8;
        seq_t coeff3 = (seq_t)3 * m_seq
                     + (seq_t)7;

        seq_t term1 = coeff1 * cache[m];
        seq_t term2 = coeff2 * cache[m + 1];
        seq_t term3 = coeff3 * cache[m + 2];

        cache[i] = term1 - term2 + term3;
    }

    last_index = n;
    return cache[n];
}

/*
 * A170531  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * * a(n) = p(n) + 40 *
 *            (a(n-1)+a(n-2)+...+a(n-45)) - 820 * a(n-46) * * where * * p(0) =
 *            1 * p(1) = 2 * p(2) = 2 * … … * p(45) = 2 * p(46) = 1 * p(n) = 0
 *            for n > 46 * * This recurrence follows from the rational
 *            generating function * * G(t) = (t^46 + 2*t^45 + 2*t^44 +...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* array of a(k) for 0 <= k < cache_len */
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of valid entries */

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to requested n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t p = 0;
        if (i == 0)          p = 1;
        else if (i <= 45)    p = 2;
        else if (i == 46)    p = 1;
        /* else p remains 0 */

        /* sum of last 45 terms a(i-1) .. a(i-45) */
        seq_t sum45 = 0;
        for (idx_t j = 1; j <= 45 && i >= j; ++j)
            sum45 += cache[i - j];

        /* term a(i-46) */
        seq_t a46 = (i >= 46) ? cache[i - 46] : 0;

        /* recurrence */
        cache[i] = p + (seq_t)40 * sum45 - (seq_t)820 * a46;
    }
    cache_len = (n + 1);

    return cache[n];
}

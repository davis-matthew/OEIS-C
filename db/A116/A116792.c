/*
 * A116792  Number of permutations of length n which avoid the patterns 1432,
 *          2134, 2341.
 * https://oeis.org/A116792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) – 18*a(n-2) + 21*a(n-3) – 12*a(n-4) + 7*a(n-5) –
 *            8*a(n-6) + 5*a(n-7) – a(n-8) + e(n), where e(n) =
 *            {0,1,–5,10,–6,2,–9,5,3,–4,1} for 1 ≤ n ≤ 10 and e(n)=0 for n>10.
 *            The recurrence comes from the rational generating function in
 *            the OEIS entry. The sequence is stored in a static cach...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated capacity (elements)       */
    static idx_t cache_used = 0;     /* highest index computed internally */

    /* allocate or expand cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)(n < 7 ? 8 : n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
    }
    if (n >= cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first eight terms (n=0..7) if not yet done */
    if (cache_used < 7) {
        cache[0] = 0ULL;
        cache[1] = 1ULL;
        cache[2] = 2ULL;
        cache[3] = 6ULL;
        cache[4] = 21ULL;
        cache[5] = 71ULL;
        cache[6] = 219ULL;
        cache[7] = 635ULL;
        cache_used = 7;
    }

    /* pre‑compute the inhomogeneous part e(n) for 1≤n≤10   */
    static const int e[11] = {0, 1, -5, 10, -6, 2, -9, 5, 3, -4, 1};

    /* compute terms up to requested n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t pos = 0;   /* sum of positive‑coefficient terms */
        seq_t neg = 0;   /* sum of negative‑coefficient terms */

        /* positive coefficients: 7, 21, 7, 5 */
        pos += cache[i - 1] * (seq_t)7;
        pos += cache[i - 3] * (seq_t)21;
        pos += cache[i - 5] * (seq_t)7;
        pos += cache[i - 7] * (seq_t)5;

        /* negative coefficients: 18, 12, 8, 1 */
        neg += cache[i - 2] * (seq_t)18;
        neg += cache[i - 4] * (seq_t)12;
        neg += cache[i - 6] * (seq_t)8;
        neg += cache[i - 8] * (seq_t)1;

        seq_t val = pos - neg;          /* enforce order to avoid underflow */

        /* add the e(i) term (only for i ≤ 10) */
        if (i <= 10) {
            if (e[i] > 0)
                val += (seq_t)e[i];
            else
                val -= (seq_t)(-e[i]);
        }

        cache[i] = val;
    }
    cache_used = (n > cache_used) ? n : cache_used;
    return cache[n];
}

/*
 * A168831  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = (t^20 + 2*t^19 + … + 2*t + 1)/(36*t^20 – 8*t^19 – … – 8*t
 *            + 1), hence a(n) satisfies a(n) = 8∑_{i=1}^{19} a(n–i) –
 *            36·a(n–20) + δ(n), with δ(20)=1, δ(0)=1, δ(1…19)=2 and δ(n)=0
 *            for n>20. This implementation pre‑computes the first 20 terms
 *            (a(0)…a(19)) by a(0)=1 and a(n)=10·9^(n–1) (1≤n≤19)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* array of computed a(i) */
    static idx_t last        = 0;          /* largest index already computed */
    static seq_t sum19       = 0;          /* sum of the last 19 values */
    static size_t capacity   = 0;          /* allocated size of cache */

    /* --- initialize for the first call --------------------------------- */
    if (cache == NULL) {
        /* reserve at least 20 entries (indices 0…19) */
        capacity = 20;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));

        /* a(0) = 1, a(n) = 10·9^(n–1) for 1 ≤ n ≤ 19 */
        cache[0] = (seq_t)1;
        seq_t val = (seq_t)10;
        for (idx_t i = 1; i < 20; ++i) {
            cache[i] = val;
            val *= (seq_t)9;
        }
        last = 19;

        /* compute sum of the last 19 values (a(1)…a(19)) */
        sum19 = (seq_t)0;
        for (idx_t i = 1; i <= 19; ++i)
            sum19 += cache[i];
    }

    /* --- extend cache up to requested n -------------------------------- */
    if (n > last) {
        /* grow array if necessary (double the current capacity) */
        while ((idx_t)(capacity - 1) < n) {
            size_t new_cap = capacity * 2;
            if (new_cap < (size_t)(n + 1))
                new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* should not happen in the test harness */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            capacity = new_cap;
        }

        /* compute values from last+1 up to n */
        for (idx_t idx = last + 1; idx <= n; ++idx) {
            /* δ(n) is non‑zero only for n = 0 or 20 */
            seq_t delta = 0;
            if (idx == 20)
                delta = (seq_t)1;

            /* recurrence: a(idx) = 8·sum19 – 36·a(idx–20) + delta */
            seq_t term = (seq_t)8 * sum19 - (seq_t)36 * cache[idx - 20] + delta;

            cache[idx] = term;

            /* update sliding sum of the last 19 values */
            sum19 += term;               /* add newest */
            sum19 -= cache[idx - 19];    /* remove the one leaving the window */

            last = idx;
        }
    }

    return cache[n];
}

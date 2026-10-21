/*
 * A094266  LQTL Lean Quaternary Temporal Logic: a terse form of temporal
 *          logic created by assigning four descriptors such that false,
 *          becoming true, true and becoming false are represented and become
 *          a linear sequence. In a branching tree two alternative are open,
 *          change or no change. The integer sequence a...
 * https://oeis.org/A094266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is obtained by iterating the linear transformation T:
 *            (a,b,c,d) → (d+a, a+b, b+c, c+d) starting from (1,0,0,0). The
 *            nth term is the component at position (n mod 4) of the tuple T_k
 *            with index k = 0 if n = 0 k = 1 if 1 ≤ n ≤ 4 k = 2 if 5 ≤ n ≤ 7
 *            k = 3 + (n‑8)/4 if n ≥ 8
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for precomputed tuples */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of tuples stored (cache_cap == k+1) */

    /* initialise the base tuple T0 = (1,0,0,0) */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = (seq_t)0;
    }

    /* quick return for a(0) */
    if (n == 0) {
        return cache[0];
    }

    /* determine the required maximum tuple index */
    size_t needed_k;
    if (n <= 4) {
        needed_k = 1;
    } else if (n < 8) {
        needed_k = 2;
    } else {
        needed_k = 3 + (n - 8) / 4;
    }

    /* grow cache if necessary */
    if (needed_k >= cache_cap) {
        size_t new_cap = needed_k + 1;   /* new number of tuples */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * 4 * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = new_cache;
        /* compute new tuples T_i for i = cache_cap … needed_k */
        for (size_t i = cache_cap; i <= needed_k; ++i) {
            seq_t a0 = cache[(i - 1) * 4 + 3] + cache[(i - 1) * 4 + 0];
            seq_t a1 = cache[(i - 1) * 4 + 0] + cache[(i - 1) * 4 + 1];
            seq_t a2 = cache[(i - 1) * 4 + 1] + cache[(i - 1) * 4 + 2];
            seq_t a3 = cache[(i - 1) * 4 + 2] + cache[(i - 1) * 4 + 3];
            cache[i * 4 + 0] = a0;
            cache[i * 4 + 1] = a1;
            cache[i * 4 + 2] = a2;
            cache[i * 4 + 3] = a3;
        }
        cache_cap = new_cap;
    }

    /* identify the tuple index k for this n */
    size_t k;
    if (n <= 4) {
        k = 1;
    } else if (n < 8) {
        k = 2;
    } else {
        k = 3 + (n - 8) / 4;
    }

    size_t comp = n % 4;   /* component position in the tuple */
    return cache[k * 4 + comp];
}

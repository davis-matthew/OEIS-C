/*
 * A209557  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209558; see the Formula section.
 * https://oeis.org/A209557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerated from flattened coefficients of polynomials
 *            u(k,x) defined by u(1)=1, v(1)=1; u(k)=x*u(k-1)+v(k-1),
 *            v(k)=u(k-1)+2x*v(k-1)+1 (k>1). The sequence a is the row‑major
 *            list of constant–to–highest‑degree coefficients of u(k,x)
 * Range:     u8 n <= 39, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1 */

    /* static state for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0, cache_cap = 0;
    static seq_t *u_prev = NULL, *v_prev = NULL;
    static idx_t prev_k = 0;

    /* initialise on first call */
    if (prev_k == 0) {
        prev_k = 1;
        u_prev = (seq_t*)malloc(sizeof(seq_t));
        u_prev[0] = (seq_t)1;
        v_prev = (seq_t*)malloc(sizeof(seq_t));
        v_prev[0] = (seq_t)1;

        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = u_prev[0];
        cache_len = 1;
    }

    /* grow cache until it holds at least n terms */
    while (cache_len < n) {
        idx_t new_k = prev_k + 1;
        seq_t *u_new = (seq_t*)malloc(sizeof(seq_t)*new_k);
        seq_t *v_new = (seq_t*)malloc(sizeof(seq_t)*new_k);

        for (idx_t i = 0; i < new_k; ++i) {
            seq_t u_shift = (i > 0 && i-1 < prev_k) ? u_prev[i-1] : 0;
            seq_t v_same  = (i < prev_k) ? v_prev[i] : 0;
            u_new[i] = u_shift + v_same;

            seq_t u_same  = (i < prev_k) ? u_prev[i] : 0;
            seq_t v_shift = (i > 0 && i-1 < prev_k) ? (seq_t)2 * v_prev[i-1] : 0;
            v_new[i] = u_same + v_shift + (i == 0 ? (seq_t)1 : (seq_t)0);
        }

        /* append new row's coefficients to cache */
        if (cache_len + new_k > cache_cap) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap < cache_len + new_k) new_cap = cache_len + new_k;
            cache = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
            cache_cap = new_cap;
        }
        for (idx_t i = 0; i < new_k; ++i) {
            cache[cache_len++] = u_new[i];
        }

        free(u_prev);
        free(v_prev);
        u_prev = u_new;
        v_prev = v_new;
        prev_k = new_k;
    }

    return cache[n-1];
}

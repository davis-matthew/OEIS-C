/*
 * A239644  Number of nX3 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays with entries in {0,1,2,3} such that
 *            no element equals the sum of the elements to its left (mod 4)
 *            nor equals two plus the sum of the elements above it (mod 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Dynamic programming over rows. */
seq_t a(idx_t n)
{
    /* one‑time static data */
    static bool             initialized = false;
    static unsigned char    trans[64][64];
    static seq_t           *a_cache    = NULL;
    static idx_t            cache_cap  = 0;
    static idx_t            last_n     = 0;            /* rows already processed */
    static seq_t            v[64];                    /* current state counts */
    static seq_t            v_next[64];

    /* initialise transition matrix once */
    if (!initialized) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                trans[i][j] = 0;

        /* pre‑compute allowed transitions for a single row          */
        for (int oa = 0; oa < 4; ++oa)            /* previous sums mod‑4 */
            for (int ob = 0; ob < 4; ++ob)
                for (int oc = 0; oc < 4; ++oc) {
                    int old_idx = oa * 16 + ob * 4 + oc;
                    for (int x1 = 0; x1 < 4; ++x1) {
                        if (x1 == 0) continue;              /* col 1 cannot be 0 (sum_left = 0) */
                        if (x1 == ((oa + 2) & 3)) continue; /* forbidden by above rule */
                        int na = (oa + x1) & 3;
                        for (int x2 = 0; x2 < 4; ++x2) {
                            if (x2 == x1) continue;           /* cannot equal left (x1) */
                            if (x2 == ((ob + 2) & 3)) continue;/* forbidden by above rule */
                            int nb = (ob + x2) & 3;
                            for (int x3 = 0; x3 < 4; ++x3) {
                                int left = (x1 + x2) & 3;
                                if (x3 == left) continue;          /* cannot equal left sum */
                                if (x3 == ((oc + 2) & 3)) continue;/* forbidden by above rule */
                                int nc = (oc + x3) & 3;
                                int new_idx = na * 16 + nb * 4 + nc;
                                trans[old_idx][new_idx] += 1;
                            }
                        }
                    }
                }

        /* initialise DP state for 0 rows */
        for (int i = 0; i < 64; ++i) v[i] = 0;
        v[0] = 1;                          /* only one way to have nothing */

        /* cache for answers */
        cache_cap = 16;
        a_cache   = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!a_cache) exit(1);
        a_cache[0] = 0;
        a_cache[1] = 0;                    /* will be overwritten when first row processed */

        initialized = true;
    }

    /* if requested n already computed */
    if (n <= last_n) return a_cache[n];

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* dynamic‑program incremental extension */
    for (idx_t step = last_n + 1; step <= n; ++step) {
        /* clear next state */
        for (int i = 0; i < 64; ++i) v_next[i] = 0;

        /* transition from each previous state */
        for (int i_state = 0; i_state < 64; ++i_state) {
            seq_t val = v[i_state];
            if (val == 0) continue;
            for (int j_state = 0; j_state < 64; ++j_state) {
                unsigned char t = trans[i_state][j_state];
                if (!t) continue;
                if (sizeof(seq_t) <= 8) {                /* use 128‑bit temporaries for small built‑in types */
                    unsigned __int128 prod = (unsigned __int128)val * t;
                    v_next[j_state] = (seq_t)((unsigned __int128)v_next[j_state] + prod);
                } else {                                 /* arbitrary‑precision or 128‑bit types */
                    v_next[j_state] += val * (seq_t)t;
                }
            }
        }

        /* copy back to current state */
        for (int i = 0; i < 64; ++i) v[i] = v_next[i];

        /* sum counts for all states to get a(step) */
        seq_t sum = 0;
        for (int i = 0; i < 64; ++i) sum += v[i];
        a_cache[step] = sum;
    }

    last_n = n;
    return a_cache[n];
}

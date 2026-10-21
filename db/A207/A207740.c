/*
 * A207740  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Count n×7 binary arrays avoiding the forbidden patterns  
   horizontally: 000 and 010;  
   vertically: 001 and 100.  
   Computed by dynamic programming over the last two rows.  */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n) {
    /* Static data to be initialized once. */
    static int R = 0;                         /* number of admissible row masks   */
    static int allowedMasks[128];             /* mask for each admissible row    */
    static int maskIndex[128];                /* mapping from full mask to 0..R-1 */
/*  static unsigned char trans[128][128][128];*/   /* transition table (0/1) */
    static uint8_t    *trans;                 /* dynamic 3‑D table accessed as t[i][j][k] = trans[(i*R + j)*R + k] */

    static seq_t cache[20001];                /* cached a(n) values (1‑based)    */
    static size_t cache_size = 0;             /* number of cached entries        */

    static seq_t dp_prev[128*128];            /* DP for previous step            */
    static seq_t dp_next[128*128];            /* DP for next  step               */
    static idx_t current_n = 0;               /* largest n that has been cached   */
    static int initialized = 0;

    /* One‑time initialization. */
    if (!initialized) {
        /* 1. Compute admissible row masks. */
        int idx = 0;
        for (int mask = 0; mask < 128; ++mask) {
            int ok = 1;
            for (int i = 0; i <= 4; ++i) {
                int b0 = (mask >> i) & 1;
                int b1 = (mask >> (i+1)) & 1;
                int b2 = (mask >> (i+2)) & 1;
                if ((b0==0 && b1==0 && b2==0) || (b0==0 && b1==1 && b2==0)) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                allowedMasks[idx] = mask;
                maskIndex[mask]  = idx;
                ++idx;
            } else {
                maskIndex[mask] = -1;
            }
        }
        R = idx;     /* number of admissible rows */

        /* 2. Allocate and fill transition table. */
        size_t trans_size = (size_t)R * R * R;
        trans = (uint8_t*)malloc(trans_size);
        for (size_t i = 0; i < (size_t)R; ++i) {
            for (size_t j = 0; j < (size_t)R; ++j) {
                for (size_t k = 0; k < (size_t)R; ++k) {
                    int m1 = allowedMasks[i];
                    int m2 = allowedMasks[j];
                    int m3 = allowedMasks[k];
                    int ok = 1;
                    for (int p = 0; p < 7; ++p) {
                        int b1 = (m1 >> p) & 1;
                        int b2 = (m2 >> p) & 1;
                        int b3 = (m3 >> p) & 1;
                        if ((b1==0 && b2==0 && b3==1) ||
                            (b1==1 && b2==0 && b3==0)) {
                            ok = 0;
                            break;
                        }
                    }
                    trans[(i*R + j)*R + k] = (uint8_t)ok;
                }
            }
        }

        /* 3. Initialise cache array to zero (not strictly necessary). */
        for (size_t i = 0; i <= 20000; ++i) cache[i] = 0;
        cache_size = 20000 + 1;  /* we have space for indices 0..20000 */
        initialized = 1;
    }

    /* Ensure we have cached up to the requested n. */
    if (n <= 0) return 0;  /* undefined for n < 1 */

    /* If n already computed, return cached value. */
    if (n <= current_n)
        return cache[n];

    /* Compute new values up to n. */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        if (m == 1) {
            cache[1] = (seq_t)R;
        } else if (m == 2) {
            cache[2] = (seq_t)R * (seq_t)R;
            /* initialize DP for two rows: all pairs are allowed. */
            for (size_t i = 0; i < (size_t)R * R; ++i)
                dp_prev[i] = 1;
        } else {
            /* Zero dp_next. */
            for (size_t i = 0; i < (size_t)R * R; ++i)
                dp_next[i] = 0;

            /* Transition from dp_prev to dp_next. */
            for (size_t i = 0; i < (size_t)R; ++i) {
                for (size_t j = 0; j < (size_t)R; ++j) {
                    seq_t w = dp_prev[i*R + j];
                    if (!w) continue;
                    for (size_t k = 0; k < (size_t)R; ++k) {
                        if (trans[(i*R + j)*R + k]) {
                            dp_next[j*R + k] += w;
                        }
                    }
                }
            }

            /* Swap dp_next into dp_prev for the next iteration. */
            for (size_t i = 0; i < (size_t)R * R; ++i)
                dp_prev[i] = dp_next[i];

            /* Sum all entries to obtain a(m). */
            seq_t total = 0;
            for (size_t i = 0; i < (size_t)R * R; ++i)
                total += dp_prev[i];
            cache[m] = total;
        }
    }

    current_n = n;
    return cache[n];
}

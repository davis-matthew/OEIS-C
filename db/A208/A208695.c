/*
 * A208695  Number of nX5 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A208695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208695: Number of n×5 0‑1 arrays avoiding 001,100 horizontally
 * and 010,101 vertically.
 *
 * Recurrence (using DP over last two rows of the array):
 *   a(1) = 14                       (all admissible 5‑bit rows)
 *   a(2) = 14² = 196                (all pairs of admissible rows)
 *   for n ≥ 3:
 *       a(n) = sum_{i,j} sum_{k}  mult[(i,j,k)] * a(n-1)_{i,j}
 *   where mult[(i,j,k)] = 1 iff the triple (i,j,k) has no forbidden
 *   vertical 3‑tuple in any column.
 *
 * The implementation enumerates the 14 admissible 5‑bit row patterns,
 * precomputes a transition table for all triples (i,j,k),
 * and uses dynamic programming to extend the array row by row.
 * Results are cached so that repeated calls with arbitrary n are
 * answered in O(n) time and O(1) amortised space.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* offset is 1 – undefined for n<1 */

    /* ---------- static data ----------------------------------- */
    static bool initialized = false;
    static size_t M;                     /* number of admissible 5‑bit rows */
    static uint8_t bits[32][5];          /* bit matrix for each row pattern */
    static size_t S;                     /* M*M, number of (i,j) states */
    static unsigned char *can = NULL;    /* transition table (i,j,k) → 1/0 */
    static seq_t *dp_prev = NULL;        /* DP table for length cur‑1 */
    static seq_t *dp_curr = NULL;        /* DP table for length cur   */
    static seq_t *cache = NULL;          /* a(n) values for n up to cache_cap */
    static size_t cache_cap = 0;         /* current cache capacity (in n) */
    static idx_t computed_n = 0;         /* largest n that has been computed */

    /* ---------- initialise static data -------------------------- */
    if (!initialized) {
        /* 1. Enumerate all 5‑bit patterns avoiding 001 and 100 */
        M = 0;
        for (uint32_t v = 0; v < 32; ++v) {
            bool ok = true;
            for (int r = 0; r <= 2; ++r) {
                int b0 = (v >> r)       & 1;
                int b1 = (v >> (r + 1)) & 1;
                int b2 = (v >> (r + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 1 && b1 == 0 && b2 == 0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                for (int c = 0; c < 5; ++c)
                    bits[M][c] = (uint8_t)((v >> c) & 1);
                ++M;
            }
        }

        /* 2. Pre‑compute the transition table
         *    can[(i*M + j)*M + k] == 1 iff (i,j,k) satisfies vertical rules. */
        S = M * M;
        can = (unsigned char *)malloc(S * M * sizeof(unsigned char));
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < M; ++j) {
                size_t base = (i * M + j) * M;
                for (size_t k = 0; k < M; ++k) {
                    bool ok = true;
                    for (int col = 0; col < 5 && ok; ++col) {
                        int b0 = bits[i][col];
                        int b1 = bits[j][col];
                        int b2 = bits[k][col];
                        if ((b0 == 0 && b1 == 1 && b2 == 0) ||
                            (b0 == 1 && b1 == 0 && b2 == 1))
                            ok = false;
                    }
                    can[base + k] = ok ? 1 : 0;
                }
            }
        }

        /* 3. Allocate DP tables */
        dp_prev = (seq_t *)malloc(S * sizeof(seq_t));
        dp_curr = (seq_t *)malloc(S * sizeof(seq_t));
        for (size_t s = 0; s < S; ++s) {
            dp_prev[s] = 0;
            dp_curr[s] = 0;
        }

        /* 4. Cache for a(n) */
        cache_cap = 4;                                 /* allow n up to 3 immediately */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = 0;                                    /* unused, for safety */
        cache[1] = (seq_t)M;                             /* a(1) */
        cache[2] = (seq_t)M * (seq_t)M;                  /* a(2) */
        computed_n = 2;

        /* 5. Initialise dp_prev for n = 2: each pair of rows
         *    is allowed because no vertical restriction applies with less than 3 rows. */
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < M; ++j)
                dp_prev[i * M + j] = 1;

        initialized = true;
    }

    /* ---------- extend cache if necessary ------------------------ */
    if ((idx_t)n > computed_n) {
        /* Resize cache vector if needed */
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)n > new_cap) new_cap <<= 1;
            seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
            for (size_t i = 0; i <= cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* DP from computed_n + 1 up to n */
        for (idx_t cur = computed_n + 1; cur <= n; ++cur) {
            /* 0: clear dp_curr */
            for (size_t s = 0; s < S; ++s)
                dp_curr[s] = 0;

            /* 1: transition */
            for (size_t s = 0; s < S; ++s) {
                seq_t val = dp_prev[s];
                if (val == 0) continue;
                size_t i = s / M;
                size_t j = s % M;
                size_t base = s * M;
                unsigned char *row_base = &can[base];
                for (size_t k = 0; k < M; ++k)
                    if (row_base[k])
                        dp_curr[j * M + k] += val;
            }

            /* 2: swap dp tables */
            seq_t *tmp = dp_prev;
            dp_prev   = dp_curr;
            dp_curr   = tmp;

            /* 3: sum all states to obtain a(cur) */
            seq_t total = 0;
            for (size_t s = 0; s < S; ++s)
                total += dp_prev[s];
            cache[cur] = total;
        }
        computed_n = n;
    }

    return cache[n];
}

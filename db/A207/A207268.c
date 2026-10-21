/*
 * A207268  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 34, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207268: Number of n×7 0/1 arrays avoiding horizontal
 *  triples 000 and 011, and vertical triples 011 and 101.
 *  The code implements a dynamic‑programming recurrence that
 *  counts admissible rows (25 in total) and transitions between
 *  successive rows.  It memoises all computed values up to the
 *  largest argument requested so that subsequent calls are O(1).
 *  The implementation is fully compatible with the build of
 *  seq_t/idx_t supplied by the test harness.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 25   /* number of admissible 7‑bit rows (no 000, no 011) */

extern seq_t a(idx_t n);   /* external declaration provided by the build */

seq_t a(idx_t n)
{
    /* ---------- static data ---------------------------------------- */
    static bool initialized = false;
    static uint8_t      masks[MAX_ROWS];           /* admissible row masks */
    static bool         next_ok[MAX_ROWS][MAX_ROWS][MAX_ROWS];
    static seq_t        cur[MAX_ROWS][MAX_ROWS];   /* DP for current length */
    static seq_t        nxt[MAX_ROWS][MAX_ROWS];   /* DP for next length  */
    static seq_t       *cache = NULL;              /* memoised a(k) */
    static size_t       cache_len = 0;             /* size of cache array  */
    static size_t       curr_len = 0;              /* largest n computed */
    static size_t       V = 0;                     /* number of admissible rows */

    /* ---------- initialise on first call --------------------------- */
    if (!initialized) {
        /* 1. enumerate admissible 7‑bit rows (no 000, no 011) */
        V = 0;
        for (uint32_t m = 0; m < 128; ++m) {
            bool ok = true;
            for (int i = 0; i <= 4; ++i) {
                uint8_t b1 = (m >> i) & 1U;
                uint8_t b2 = (m >> (i + 1)) & 1U;
                uint8_t b3 = (m >> (i + 2)) & 1U;
                if ((b1 == 0U && b2 == 0U && b3 == 0U) ||
                    (b1 == 0U && b2 == 1U && b3 == 1U)) {
                    ok = false;
                    break;
                }
            }
            if (ok) masks[V++] = (uint8_t)m;
        }

        /* 2. pre‑compute transition table respecting vertical constraints */
        memset(next_ok, 0, sizeof(next_ok));
        for (size_t i = 0; i < V; ++i) {
            for (size_t j = 0; j < V; ++j) {
                for (size_t k = 0; k < V; ++k) {
                    bool ok = true;
                    for (int col = 0; col < 7; ++col) {
                        uint8_t a = (masks[i] >> col) & 1U;
                        uint8_t b = (masks[j] >> col) & 1U;
                        uint8_t c = (masks[k] >> col) & 1U;
                        /* forbid 011 and 101 vertically */
                        if (c == 1U && a != b) {
                            ok = false;
                            break;
                        }
                    }
                    next_ok[i][j][k] = ok;
                }
            }
        }

        /* 3. allocate cache and initialise first two terms */
        cache_len = 4;                /* accommodate indices 0..3 at least */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) { exit(1); }      /* abort on allocation failure */
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)V;          /* a(1) = 25 */
        cache[2] = (seq_t)V * (seq_t)V;/* a(2) = 625 */

        /* 4. initialise DP state for length 2: one way for every pair */
        for (size_t i = 0; i < V; ++i)
            for (size_t j = 0; j < V; ++j)
                cur[i][j] = (seq_t)1;

        curr_len = 2;
        initialized = true;
    }

    /* ---------- trivial and cached cases -------------------------- */
    if (n == 0) return (seq_t)0;        /* not defined, return 0 */
    if (n <= curr_len) return cache[(size_t)n];

    /* ---------- extend DP up to requested n ---------------------- */
    for (idx_t len = (idx_t)curr_len + 1; len <= n; ++len) {
        /* zero nxt */
        for (size_t i = 0; i < V; ++i)
            for (size_t j = 0; j < V; ++j)
                nxt[i][j] = (seq_t)0;

        /* transitions */
        for (size_t i = 0; i < V; ++i) {
            for (size_t j = 0; j < V; ++j) {
                seq_t ways = cur[i][j];
                if (!ways) continue;
                for (size_t k = 0; k < V; ++k) {
                    if (next_ok[i][j][k]) {
                        nxt[j][k] += ways;
                    }
                }
            }
        }

        /* swap cur and nxt */
        for (size_t i = 0; i < V; ++i)
            for (size_t j = 0; j < V; ++j)
                cur[i][j] = nxt[i][j];

        /* sum all ways for this length */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < V; ++i)
            for (size_t j = 0; j < V; ++j)
                total += cur[i][j];

        /* ensure cache has space for this n */
        if ((size_t)(n + 1) > cache_len) {
            size_t new_len = cache_len * 2;
            if (new_len < (size_t)(n + 1))
                new_len = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) { exit(1); }    /* abort on allocation failure */
            cache = tmp;
            cache_len = new_len;
        }

        cache[(size_t)len] = total;
        curr_len = (size_t)len;
    }

    return cache[(size_t)n];
}

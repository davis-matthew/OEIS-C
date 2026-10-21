/*
 * A208288  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A208288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 4*a(n-3) - 2*a(n-4) + a(n-5)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently computed by a transfer‑matrix DP over 4‑row columns) */

#include <stdlib.h>
#include <stdbool.h>

/* The following helper may be omitted if seq_t is a plain integer type:
   it is only provided for safety with arbitrary‑precision builds. */
#ifndef seq_alloc
#  define seq_alloc(n)  ((seq_t*)malloc((n)*sizeof(seq_t)))
#  define seq_free(p)   (free((void*)(p)))
#endif

/* Global state of the memoised algorithm */
static bool      initialized = false;
static seq_t*    cache       = NULL;       /* cache[1..] */
static size_t    cache_cap   = 0;          /* allocated size of cache (>=max cached n) */
static idx_t    cached_max = 2;            /* largest n for which cache is filled */

/* DP state for the transfer‑matrix */
static seq_t    dp[256];
static seq_t    ndp[256];
static idx_t    allowed_cols[8];                      /* all vertical‑valid 4‑bit columns   */
static bool     validTri[16][16][16];                 /* horizontal test for triples of columns */

/* ----------------------------------------------------------------- */
/* initialise all tables and data structures                         */
static void init(void)
{
    /* ---- 1. Find all vertical‑valid columns (no 001 or 110 in any 3 consecutive rows) */
    int cnt = 0;
    for (idx_t col = 0; col < 16; ++col) {
        bool ok = true;
        /* check rows 0‑2 and 1‑3 (4 rows, 3‑tuples in each) */
        for (int r = 0; r < 2; ++r) {
            int t = ((col >> r) & 1) << 2 | ((col >> (r + 1)) & 1) << 1 | ((col >> (r + 2)) & 1);
            if (t == 1 || t == 6) { ok = false; break; }
        }
        if (ok) allowed_cols[cnt++] = col;
    }

    /* ---- 2. Pre‑compute horizontal validity of every triple of columns */
    for (idx_t a = 0; a < 16; ++a)
        for (idx_t b = 0; b < 16; ++b)
            for (idx_t c = 0; c < 16; ++c) {
                bool ok = true;
                for (int r = 0; r < 4; ++r) {          /* each of the 4 rows */
                    int t = ((a >> r) & 1) << 2 |
                            ((b >> r) & 1) << 1 |
                            ((c >> r) & 1);
                    if (t == 0 || t == 1) { ok = false; break; }   /* 000 or 001 forbidden */
                }
                validTri[a][b][c] = ok;
            }

    /* ---- 3. Build initial DP for n = 2 (two columns) */
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    for (int i = 0; i < cnt; ++i)
        for (int j = 0; j < cnt; ++j) {
            idx_t key = (allowed_cols[i] << 4) | allowed_cols[j];
            dp[key] = 1;                       /* exactly one way to reach this pair */
        }

    /* ---- 4. Allocate cache and store a(1) and a(2) */
    cache_cap = 8;                              /* initial capacity */
    cache     = seq_alloc(cache_cap + 1);        /* index 0 unused */
    cache[0]  = 0;
    cache[1]  = 8;               /* a(1) */
    cache[2]  = 64;              /* a(2) */
    cache[3]  = 0;               /* placeholder for a(3) once computed */

    initialized = true;
}

/* ----------------------------------------------------------------- */
/* main sequence function                                            */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* Reach the desired n by extending DP and cache as necessary */
    while (cached_max < n) {
        /* ---- 1. Compute next step (from n == cached_max to cached_max+1) */
        for (int i = 0; i < 256; ++i) ndp[i] = 0;

        for (int i = 0; i < 256; ++i) {
            seq_t ways = dp[i];
            if (!ways) continue;

            idx_t c1 = (i >> 4) & 0xF;
            idx_t c2 =  i       & 0xF;

            /* try every possible next column (8 choices) */
            for (int k = 0; k < 8; ++k) {
                idx_t c3 = allowed_cols[k];
                if (!validTri[c1][c2][c3]) continue;

                idx_t key = (c2 << 4) | c3;
                ndp[key] += ways;
            }
        }

        /* ---- 2. Move to next state */
        for (int i = 0; i < 256; ++i) dp[i] = ndp[i];

        /* ---- 3. Sum all possibilities to get a(cached_max+1) */
        seq_t total = 0;
        for (int i = 0; i < 256; ++i) total += dp[i];

        /* ---- 4. Grow cache if necessary */
        if (cached_max + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < cached_max + 1) new_cap = cached_max + 1;
            cache = seq_alloc(new_cap + 1);
            cache_cap = new_cap;
        }
        cache[++cached_max] = total;
    }

    return cache[n];
}

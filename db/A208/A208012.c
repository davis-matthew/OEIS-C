/*
 * A208012  Number of nX7 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ n×7 0/1 arrays avoiding 000 and 101 horizontally, and
 *            001 and 011 vertically }| (offset 1) The implementation uses
 *            dynamic programming over the 2‑row state. The recurrence is:
 *            a(1) = M a(2) = M·M a(n) (n≥3) = sum of all DP values after n
 *            rows are processed, where M is the number of 7‑bi...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 27, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Configuration constants (do not modify)                           */
/* ------------------------------------------------------------------ */
#define MAX_N      10000   /* maximum n for which the cache is built   */
#define MAX_ROWS   28      /* maximum number of valid 7‑bit rows       */

/* ------------------------------------------------------------------ */
/*  Static data reused by all calls                                   */
/* ------------------------------------------------------------------ */
static bool initialized = false;

static uint8_t  rows[MAX_ROWS];                  /* mask of a valid row   */
static int      M = 0;                           /* number of valid rows  */
static unsigned char allow_cnt[MAX_ROWS][MAX_ROWS];
static unsigned char allow_list[MAX_ROWS][MAX_ROWS][MAX_ROWS];

/* DP arrays of size M*M (≤ 784)                                   */
static seq_t dp_prev[MAX_ROWS*MAX_ROWS];
static seq_t dp_cur [MAX_ROWS*MAX_ROWS];

/* Cache of previously computed a(n) values (1‑based)              */
static seq_t cache[MAX_N+1];
static idx_t cached_max = 0;

/* ------------------------------------------------------------------ */
/*  Helper to initialise the static tables only once                  */
/* ------------------------------------------------------------------ */
static void init_static(void)
{
    if (initialized) return;

    /* ---- 1. enumerate all 7‑bit rows that avoid horizontal 000 and 101  */
    int cnt = 0;
    for (int mask = 0; mask < 128; ++mask) {              /* 0 … 127 */
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {                     /* 5 possible windows */
            int pat = (mask >> j) & 0x7;                  /* 3 consecutive bits */
            if (pat == 0x0 || pat == 0x5) {               /* 000 or 101 */
                ok = false;
                break;
            }
        }
        if (ok) {
            rows[cnt++] = (uint8_t)mask;
        }
    }
    M = cnt;

    /* ---- 2. pre‑compute vertical compatibility lists  (001/011 forbidden) */
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            unsigned char kcnt = 0;
            uint8_t mask_i = rows[i];
            uint8_t mask_j = rows[j];
            for (int k = 0; k < M; ++k) {
                uint8_t mask_k = rows[k];
                bool ok = true;
                for (int col = 0; col < 7; ++col) {
                    int b1 = (mask_i >> col) & 1;
                    int b2 = (mask_j >> col) & 1;
                    int b3 = (mask_k >> col) & 1;
                    int pat = (b1 << 2) | (b2 << 1) | b3;  /* 3‑bit pattern */
                    if (pat == 0x1 || pat == 0x3) {        /* 001 or 011 */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    allow_list[i][j][kcnt++] = (unsigned char)k;
                }
            }
            allow_cnt[i][j] = kcnt;
        }
    }

    /* ---- 3. initialise DP for n = 2 (every pair of rows is possible)   */
    size_t stateCount = (size_t)M * (size_t)M;
    for (size_t s = 0; s < stateCount; ++s) {
        dp_prev[s] = (seq_t)1;
        dp_cur [s] = (seq_t)0;
    }

    /* ---- 4. initialise cache (offset 1)                              */
    cache[0] = (seq_t)0;                     /* unused, but harmless   */
    cache[1] = (seq_t)M;                     /* a(1)                    */
    cache[2] = (seq_t)M * (seq_t)M;          /* a(2)                    */
    cached_max = 2;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the kernel                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --- 1. initialise static data --------------------------------- */
    init_static();

    /* --- 2. return cached result if available ---------------------- */
    if (n <= (idx_t)cached_max) {
        return cache[n];
    }

    /* --- 3. guard against out‑of‑range requests --------------------- */
    if (n > (idx_t)MAX_N) {
        return (seq_t)0;   /* sentinel for out of bounds */
    }

    /* --- 4. compute successive terms until n ----------------------- */
    size_t stateCount = (size_t)M * (size_t)M;

    for (idx_t step = cached_max + 1; step <= n; ++step) {
        /*   a) clear current DP array */
        memset(dp_cur, 0, stateCount * sizeof(seq_t));

        /*   b) transition from every (i,j) pair to all allowed k   */
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t val = dp_prev[(size_t)i * M + j];
                if (!val) continue;
                unsigned char kcnt = allow_cnt[i][j];
                const unsigned char *klist = allow_list[i][j];
                for (unsigned char t = 0; t < kcnt; ++t) {
                    int k = klist[t];
                    size_t idx = (size_t)j * M + k;
                    dp_cur[idx] += val;
                }
            }
        }

        /*   c) sum all entries to obtain a(step) */
        seq_t total = (seq_t)0;
        for (size_t idx = 0; idx < stateCount; ++idx) {
            total += dp_cur[idx];
        }
        cache[step] = total;

        /*   d) make dp_cur the new dp_prev for the next iteration   */
        memcpy(dp_prev, dp_cur, stateCount * sizeof(seq_t));
    }

    cached_max = n;
    return cache[n];
}

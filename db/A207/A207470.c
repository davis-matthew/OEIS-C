/*
 * A207470  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 30, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207470: Number of 6×n 0/1 arrays avoiding
 * horizontally 000 and 011, vertically 010 and 101.
 * The values are computed with a transfer‑matrix DP.
 * Formula: a(n) = number of such matrices of width n. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* One‑time static initialisation data */
static bool initialized   = false;     /* flag */
static size_t max_cached   = 0;         /* largest n already computed */
static seq_t *cache        = NULL;      /* a[1..max_cached] (index 0 unused) */

/* DP arrays */
static seq_t *dp_cur = NULL;            /* counts for current length */
static seq_t *dp_next = NULL;           /* counts for next length */

/* Transition table */
#define STATE_COUNT 4096
#define MAX_TRANS 64
static uint16_t transitions[STATE_COUNT][MAX_TRANS];
static uint8_t  trans_cnt[STATE_COUNT];

/* Valid column patterns (6‑bit) avoiding vertical 010 or 101 */
static uint16_t valid_cols[32];
static uint8_t  valid_cnt = 0;

/* Pre‑computation of valid columns and transition table */
static void precompute(void)
{
    /* Find all 6‑bit columns that avoid 010 / 101 vertically */
    for (int v = 0; v < 64; ++v) {
        bool ok = true;
        for (int i = 0; i <= 3; ++i) {
            int a = (v >> i)     & 1;
            int b = (v >> (i+1)) & 1;
            int c = (v >> (i+2)) & 1;
            if ((a == c) && (a != b)) {  /* 010 or 101 */
                ok = false;
                break;
            }
        }
        if (ok) {
            valid_cols[valid_cnt++] = (uint16_t)v;
        }
    }

    /* Pre‑compute transitions for every state s (two last columns) */
    for (int s = 0; s < STATE_COUNT; ++s) {
        trans_cnt[s] = 0;
        for (int vi = 0; vi < valid_cnt; ++vi) {
            int v = valid_cols[vi];
            bool ok = true;
            for (int r = 0; r < 6; ++r) {
                int old = (s >> (2*r))     & 1;
                int mid = (s >> (2*r + 1)) & 1;
                int nb  = (v >> r)         & 1;
                int code = (old << 2) | (mid << 1) | nb;   /* 3‑bit pattern */
                if (code == 0b000 || code == 0b011) {      /* forbidden 000 or 011 */
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* Build next state: previous mid becomes new old, nb becomes new mid */
            int ns = 0;
            for (int r = 0; r < 6; ++r) {
                int mid = (s >> (2*r + 1)) & 1;
                int nb  = (v >> r)         & 1;
                ns |= (mid << (2*r));
                ns |= (nb  << (2*r + 1));
            }
            transitions[s][trans_cnt[s]++] = (uint16_t)ns;
        }
    }

    /* Allocate DP arrays */
    dp_cur  = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
    dp_next = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp_cur[i]  = 0;
        dp_next[i] = 0;
    }

    /* Initialise DP for length = 2: all pairs of valid columns */
    for (int vi = 0; vi < valid_cnt; ++vi) {
        for (int vj = 0; vj < valid_cnt; ++vj) {
            int s = 0;
            for (int r = 0; r < 6; ++r) {
                int bit1 = (valid_cols[vi] >> r) & 1;
                int bit2 = (valid_cols[vj] >> r) & 1;
                s |= (bit1 << (2*r));
                s |= (bit2 << (2*r + 1));
            }
            dp_cur[s] += 1;
        }
    }

    /* Initialise cache for n = 1 and n = 2 */
    cache      = (seq_t*)malloc(3 * sizeof(seq_t));   /* indices 0..2 */
    cache[0]   = 0;
    cache[1]   = 0;
    cache[2]   = 0;
    for (int vi = 0; vi < valid_cnt; ++vi) cache[1] += 1;          /* n = 1 */
    seq_t vv   = (seq_t)valid_cnt;
    cache[2]   = vv * vv;                                            /* n = 2 */
    max_cached = 2;

    initialized = true;
}

seq_t a(idx_t n)
{
    /* Initialisation on first use */
    if (!initialized) precompute();

    if (n == 0) return 0;          /* undefined; return 0 */

    /* Extend cache and DP if required */
    if (n > max_cached) {
        /* Allocate larger cache */
        seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= max_cached; ++i) new_cache[i] = cache[i];

        /* Free old cache */
        free(cache);
        cache = new_cache;

        /* Compute DP steps from length = max_cached+1 up to n */
        for (idx_t len = max_cached + 1; len <= n; ++len) {
            /* Zero next DP array */
            for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

            /* Transition from current to next */
            for (int s = 0; s < STATE_COUNT; ++s) {
                seq_t cnt = dp_cur[s];
                if (!cnt) continue;
                uint8_t c = trans_cnt[s];
                uint16_t *t = transitions[s];
                for (uint8_t k = 0; k < c; ++k) {
                    dp_next[t[k]] += cnt;
                }
            }

            /* Sum to get a(len) */
            seq_t total = 0;
            for (int i = 0; i < STATE_COUNT; ++i) total += dp_next[i];
            cache[len] = total;

            /* Swap dp arrays for next iteration */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }
        max_cached = n;
    }

    return cache[n];
}

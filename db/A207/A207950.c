/*
 * A207950  Number of 4 X n 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming on 4‑bit column *
 *            states. A 4‑bit state is *allowed* if no three consecutive rows
 *            * in that column contain 010 or 101 vertically. For each pair of
 *            * allowed states (i , j) we pre‑compute all allowed third states
 *            k * such that for every row the triple of bit...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ---------- Constants ---------------------------------------- */
#define STATE_BITS 4                     /* 4 rows per column       */
#define MAX_VERT_STATES (1 << STATE_BITS)  /* 16 possible columns  */
#define ALLOWED_CNT 10                   /* number of allowed columns */
#define PAIRS (ALLOWED_CNT * ALLOWED_CNT)  /* i,j pairs */
#define MAX_K   ALLOWED_CNT              /* possible k values per pair */
#define MAX_N  20000                     /* maximum n we cache */

/* ---------- static data ------------------------------------ */
static uint16_t allowed[ALLOWED_CNT];                /* allowed 4‑bit states */
static int trans_next[PAIRS][MAX_K];                 /* for each (i,j) list of k */
static int trans_cnt[PAIRS];                        /* # of valid k for each pair */
static int preinit_done = 0;

/* ---------- memoised cache --------------------------------- */
static seq_t cache_vals[MAX_N + 1] = {0};
static idx_t cache_last = 0;   /* largest n for which cache_vals[n] is set */

/* ---------- DP arrays ------------------------------------- */
static seq_t curr[PAIRS];
static seq_t next[PAIRS];

/* ---------- Helper: pre‑compute allowed states and transitions */
static void preinit(void)
{
    if (preinit_done) return;

    int ac = 0;
    /* find allowed column states */
    for (int s = 0; s < MAX_VERT_STATES; ++s) {
        int ok = 1;
        for (int r = 0; r <= STATE_BITS - 3; ++r) { /* triples 1-3, 2-4 */
            int a = (s >> r) & 1;
            int b = (s >> (r + 1)) & 1;
            int c = (s >> (r + 2)) & 1;
            if ((a == 0 && b == 1 && c == 0) || (a == 1 && b == 0 && c == 1)) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            allowed[ac++] = (uint16_t)s;
        }
    }

    /* build transitions for every pair (i,j) */
    for (int i = 0; i < ALLOWED_CNT; ++i) {
        for (int j = 0; j < ALLOWED_CNT; ++j) {
            int idx = i * ALLOWED_CNT + j;
            int ct = 0;
            for (int k = 0; k < ALLOWED_CNT; ++k) {
                int good = 1;
                for (int r = 0; r < STATE_BITS; ++r) {
                    int b1 = (allowed[i] >> r) & 1;
                    int b2 = (allowed[j] >> r) & 1;
                    int b3 = (allowed[k] >> r) & 1;
                    /* horizontal forbidden patterns 001 and 011 */
                    if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                        (b1 == 0 && b2 == 1 && b3 == 1)) {
                        good = 0;
                        break;
                    }
                }
                if (good) {
                    trans_next[idx][ct++] = k;
                }
            }
            trans_cnt[idx] = ct;
        }
    }

    preinit_done = 1;
}

/* ---------- Main function ------------------------------------ */
seq_t a(idx_t n)
{
    if (!preinit_done) preinit();

    if (n == 0) return 0;            /* undefined, but safe */

    if (n <= cache_last)             /* already computed */
        return cache_vals[(size_t)n];

    /* compute up to n */
    if (n > MAX_N) {
        /* Growth not supported; compute on the fly without caching */
        if (n == 1) return (seq_t)10;
        if (n == 2) return (seq_t)100;
    }

    /* initialise cache for n=1 */
    if (cache_last < 1) {
        cache_vals[1] = (seq_t)10;
        cache_last = 1;
    }

    /* initialise DP for length 2 if not yet done */
    if (cache_last < 2) {
        for (int i = 0; i < PAIRS; ++i) curr[i] = (seq_t)1;
        seq_t sum = 0;
        for (int i = 0; i < PAIRS; ++i) sum += curr[i];
        cache_vals[2] = sum;
        cache_last = 2;
    }

    /* extend DP until we reach n */
    for (idx_t len = cache_last + 1; len <= n; ++len) {
        memset(next, 0, sizeof(next));

        for (int idx = 0; idx < PAIRS; ++idx) {
            seq_t val = curr[idx];
            if (val == 0) continue;
            int j = idx % ALLOWED_CNT;
            int cnt = trans_cnt[idx];
            for (int t = 0; t < cnt; ++t) {
                int k = trans_next[idx][t];
                next[j * ALLOWED_CNT + k] += val;
            }
        }

        /* copy next to curr for next iteration */
        memcpy(curr, next, sizeof(curr));

        /* sum curr gives a(len) */
        seq_t sum = 0;
        for (int i = 0; i < PAIRS; ++i) sum += curr[i];
        if (len <= MAX_N) {
            cache_vals[(size_t)len] = sum;
            cache_last = len;
        }
    }

    return cache_vals[(size_t)n];
}

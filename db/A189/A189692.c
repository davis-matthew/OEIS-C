/*
 * A189692  Number of nX5 binary arrays without the pattern 0 0 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A189692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by a transfer–matrix DP for n×5 binary arrays
 *            avoiding the pattern 0 0 1 vertically, on the main diagonal, and
 *            on the anti‑diagonal. No closed formula is used. Initial values:
 *            a(1)=32, a(2)=1024, a(3)=10327, … (see OEIS A189692)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool init_done = false;
/* allowed[a][b] – bitmask of rows c that are admissible after rows a,b. */
static uint32_t allowed[32][32];
/* DP tables: dp[i][j] counts arrays ending with rows i (row n‑2), j (row n‑1). */
static seq_t dp[32][32];
static seq_t newdp[32][32];
/* Cached results a(k) for k up to cache_len-1. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
/* Number of rows already finished by DP (>=2). */
static idx_t cur_n = 0;

/* ---------------------------------------------------------------------- */
/* Check whether rows a,b,c contain a forbidden pattern.                 */
/* ---------------------------------------------------------------------- */
static bool valid_transition(int a, int b, int c)
{
    int aj, bj, cj;
    int cj1, cj2, cj3;
    int aj1, bj1, cj1d;
    int aj2, bj2, cj2d;
    int aj3, bj3, cj3d;

    /* Vertical 0 0 1 */
    for (int i = 0; i < 5; ++i) {
        aj = (a >> i) & 1;
        bj = (b >> i) & 1;
        cj = (c >> i) & 1;
        if (aj == 0 && bj == 0 && cj == 1) return false;
    }

    /* Main diagonal 0 0 1: a_i , b_{i+1} , c_{i+2}  (i = 0..2) */
    for (int i = 0; i < 3; ++i) {
        aj = (a >> i) & 1;
        bj = (b >> (i + 1)) & 1;
        cj = (c >> (i + 2)) & 1;
        if (aj == 0 && bj == 0 && cj == 1) return false;
    }

    /* Anti-diagonal 0 0 1: a_{i+2} , b_{i+1} , c_i  (i = 0..2) */
    for (int i = 0; i < 3; ++i) {
        aj = (a >> (i + 2)) & 1;
        bj = (b >> (i + 1)) & 1;
        cj = (c >> i) & 1;
        if (aj == 0 && bj == 0 && cj == 1) return false;
    }
    return true;
}

/* ---------------------------------------------------------------------- */
/* Pre‑compute the allowed transition masks and initialise DP.            */
/* ---------------------------------------------------------------------- */
static void init(void)
{
    /* Build transition masks. */
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c)
                if (valid_transition(a, b, c))
                    mask |= 1u << c;
            allowed[a][b] = mask;
        }
    }

    /* Allocate cache for indices 0..2.  index 0 unused. */
    cache_len = 3;
    cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
    cache[1] = (seq_t)32;   /* a(1) */
    cache[2] = (seq_t)1024; /* a(2) */

    /* DP state for n=2: every pair of rows is admissible. */
    for (int a = 0; a < 32; ++a)
        for (int b = 0; b < 32; ++b)
            dp[a][b] = (seq_t)1;

    cur_n = 2;
    init_done = true;
}

/* ---------------------------------------------------------------------- */
/* Extend cache up to index n (n >= 3).  DP uses previously computed state. */
/* ---------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    /* Grow cache array if necessary. */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        /* initialize new entries to zero */
        memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute values for indices > cur_n up to n. */
    for (idx_t k = cur_n + 1; k <= n; ++k) {
        memset(newdp, 0, sizeof(newdp));

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t cnt = dp[a][b];
                if (cnt == (seq_t)0) continue;
                uint32_t mask = allowed[a][b];
                while (mask) {
                    uint32_t lsb = mask & -mask;
                    int c = (int)__builtin_ctz(mask);
                    newdp[b][c] += cnt;
                    mask ^= lsb;
                }
            }
        }

        /* Sum all entries of newdp to get a(k). */
        seq_t total = (seq_t)0;
        for (int b = 0; b < 32; ++b)
            for (int c = 0; c < 32; ++c)
                total += newdp[b][c];
        cache[k] = total;

        /* Copy newdp into dp for next iteration. */
        memcpy(dp, newdp, sizeof(dp));

        cur_n = k;
    }
}

/* ---------------------------------------------------------------------- */
/* Public function required by the build.                                */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init();

    if (n == (idx_t)1) return cache[1];
    if (n == (idx_t)2) return cache[2];

    ensure_cache(n);
    return cache[n];
}

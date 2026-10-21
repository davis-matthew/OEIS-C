/*
 * A237552  Number of (n+1)X(3+1) 0..2 arrays with the maximum minus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   absdiff8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = number of (n+1)×4 arrays of 0..2 such that
   for each 2×2 subblock the value max−upper_median differs by 1 from
   those of its horizontally and vertically adjacent subblocks.
   The sequence is computed by dynamic programming over the rows. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/absdiff8.h"

/* Basic types supplied by the build */
typedef uint64_t idx_t;   /* placeholder, the real definition comes from the build */

/* ------------------------------------------------------------------ */
/* Precomputation data structures                                   */
/* ------------------------------------------------------------------ */

static bool precomp_done = false;

/* Row representations (81 rows of 4 digits 0..2) */
static uint8_t row_digits[81][4];

/* Mapping from a 3‑tuple (d0,d1,d2) to an index 0..26 (or -1 if invalid) */
static int8_t v_idx[81][81];    /* -1 for invalid pair */

/* Reverse mapping: index → (d0,d1,d2) */
static uint8_t v_vec[27][3];

/* Compatibility of two 3‑tuples: true if each component differs by exactly 1 */
static bool comp[27][27];

/* Transition table: for each state (prev_row=j, prev_vec=vPrev) list of
   possible next rows r with resulting vector vCurr. */
typedef struct { unsigned char r; unsigned char v; } trans_entry;
static trans_entry trans[81][27][81];
static uint8_t trans_count[81][27];

/* ------------------------------------------------------------------ */
/* Helper functions                                                   */

static inline unsigned char block_diff(uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3)
{
    unsigned char arr[4] = {a0, a1, a2, a3};
    /* simple sort of 4 elements */
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (arr[j] < arr[i]) {
                unsigned char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
    return arr[3] - arr[2]; /* max - second largest (= upper median) */
}

/* ------------------------------------------------------------------ */
/* Precomputation                                                    */
/* ------------------------------------------------------------------ */
static void precompute(void)
{
    if (precomp_done) return;
    precomp_done = true;

    /* Generate all 81 rows (4 digits base 3) */
    for (int idx = 0; idx < 81; ++idx) {
        int x = idx;
        for (int pos = 3; pos >= 0; --pos) {
            row_digits[idx][pos] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* Mapping vector ↔ index */
    int idx = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                v_vec[idx][0] = (uint8_t)a;
                v_vec[idx][1] = (uint8_t)b;
                v_vec[idx][2] = (uint8_t)c;
                ++idx;
            }

    /* Compute v_idx for each pair of rows */
    for (int i = 0; i < 81; ++i) {
        for (int j = 0; j < 81; ++j) {
            uint8_t diff[3];
            for (int c = 0; c < 3; ++c) {
                diff[c] = block_diff(row_digits[i][c], row_digits[i][c + 1],
                                     row_digits[j][c], row_digits[j][c + 1]);
            }
            if (absdiff8(diff[0], diff[1]) == 1 && absdiff8(diff[1], diff[2]) == 1) {
                /* Find corresponding index */
                int v = -1;
                for (int t = 0; t < 27; ++t)
                    if (v_vec[t][0] == diff[0] && v_vec[t][1] == diff[1] &&
                        v_vec[t][2] == diff[2]) {
                        v = t;
                        break;
                    }
                v_idx[i][j] = (int8_t)v;
            } else {
                v_idx[i][j] = -1;
            }
        }
    }

    /* Compatibility matrix */
    for (int v1 = 0; v1 < 27; ++v1)
        for (int v2 = 0; v2 < 27; ++v2) {
            bool ok = true;
            for (int k = 0; k < 3; ++k)
                if (absdiff8(v_vec[v1][k], v_vec[v2][k]) != 1) {
                    ok = false;
                    break;
                }
            comp[v1][v2] = ok;
        }

    /* Transition lists */
    for (int j = 0; j < 81; ++j)
        for (int vPrev = 0; vPrev < 27; ++vPrev) {
            uint8_t cnt = 0;
            for (int r = 0; r < 81; ++r) {
                int vCurr = v_idx[j][r];
                if (vCurr < 0) continue;
                if (comp[vPrev][vCurr]) {
                    trans[j][vPrev][cnt].r = (unsigned char)r;
                    trans[j][vPrev][cnt].v = (unsigned char)vCurr;
                    ++cnt;
                }
            }
            trans_count[j][vPrev] = cnt;
        }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    precompute();    /* ensure tables are ready */

    /* DP arrays */
    seq_t dp[81][27];
    seq_t newdp[81][27];

    /* zero base dp */
    for (int i = 0; i < 81; ++i)
        for (int v = 0; v < 27; ++v)
            dp[i][v] = (seq_t)0;

    /* Initial pair (rows 0 & 1) */
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j) {
            int v = v_idx[i][j];
            if (v >= 0)
                dp[j][v] += (seq_t)1;
        }

    /* Sum after first pair (n = 1) */
    seq_t total = (seq_t)0;
    for (int j = 0; j < 81; ++j)
        for (int v = 0; v < 27; ++v)
            total += dp[j][v];

    if (n == 1)
        return total;

    /* Extend for rows 2 … n (steps = n‑1) */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero newdp */
        for (int i = 0; i < 81; ++i)
            for (int v = 0; v < 27; ++v)
                newdp[i][v] = (seq_t)0;

        /* transitions */
        for (int j = 0; j < 81; ++j) {
            for (int vPrev = 0; vPrev < 27; ++vPrev) {
                seq_t cnt = dp[j][vPrev];
                if (cnt == (seq_t)0) continue;
                uint8_t tcnt = trans_count[j][vPrev];
                trans_entry *list = trans[j][vPrev];
                for (uint8_t t = 0; t < tcnt; ++t) {
                    unsigned r = list[t].r;
                    unsigned vCurr = list[t].v;
                    newdp[r][vCurr] += cnt;
                }
            }
        }

        /* copy newdp → dp */
        for (int i = 0; i < 81; ++i)
            for (int v = 0; v < 27; ++v)
                dp[i][v] = newdp[i][v];
    }

    /* Final total */
    seq_t result = (seq_t)0;
    for (int j = 0; j < 81; ++j)
        for (int v = 0; v < 27; ++v)
            result += dp[j][v];

    return result;
}

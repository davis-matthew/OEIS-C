/*
 * A237468  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   compute_s, decode4, encode4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A237468: Number of (n+1)×(5) 0..2 arrays with the maximum plus the
 * lower median minus the upper median of every 2×2 subblock differing from
 * its horizontal and vertical neighbors by exactly one.  The values are
 * computed by a dynamic programming over the rows of the array.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/compute_s.h"
#include "../utils/decode4.h"
#include "../utils/encode4.h"

/* 3^5 possible 5‑digit rows, 3^4 possible 4‑digit patterns.  */
#define ROWS     243       /* 3^5 */
#define PATCNT   81        /* 3^4 */
#define H_V      8         /* number of horizontal‑valid 4‑digit patterns */
#define STATE    (ROWS * H_V)

/* ------------------------------------------------------------------ */
/* helpers for the subblock value s = max + lower median – upper median */

/* ------------------------------------------------------------------ */
/* globals used only inside this file                                  */
/* ------------------------------------------------------------------ */
static uint8_t  rowv[ROWS][5];          /* each 5‑digit row in base‑3 */
static uint8_t  s_pat[PATCNT][4];       /* the four digits of a 4‑digit pattern */
static bool     hvalid[PATCNT];         /* true if horizontal differences =1 */
static uint8_t  plist[H_V];             /* list of valid pattern codes */
static uint8_t  rev_map[PATCNT];        /* code → index in plist or 255 */
static uint8_t  vert_adj[H_V][H_V];    /* vertical neighbours of a pattern */
static uint8_t  vert_cnt[H_V];          /* number of vertical neighbours */
static uint16_t count_arr[STATE];       /* number of next rows per state */
static size_t  *offset_arr = NULL;      /* start of each state's list of next rows */
static uint16_t *nxt_rows  = NULL;      /* flattened array of next rows */
static seq_t   dp_cur[STATE];
static seq_t   dp_nxt[STATE];

/* ------------------------------------------------------------------ */
/* build all tables (executed once)                                    */
/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    /* 1. all 5‑digit rows */
    for (uint16_t i = 0; i < ROWS; ++i) {
        uint32_t t = i;
        for (int j = 4; j >= 0; --j) rowv[i][j] = t % 3, t /= 3;
    }

    /* 2. 4‑digit patterns and their horizontal validity */
    uint8_t patIdx = 0;
    for (uint32_t code = 0; code < PATCNT; ++code) {
        decode4((uint8_t)code, s_pat[code]);

        uint8_t d0 = s_pat[code][0], d1 = s_pat[code][1],
                d2 = s_pat[code][2], d3 = s_pat[code][3];
        hvalid[code] =
            (abs((int)d0 - (int)d1) == 1) &&
            (abs((int)d1 - (int)d2) == 1) &&
            (abs((int)d2 - (int)d3) == 1);

        if (hvalid[code]) {
            plist[patIdx++] = (uint8_t)code;
            rev_map[code]   = patIdx - 1;
        } else {
            rev_map[code] = 255;
        }
    }

    /* 3. vertical neighbour lists */
    for (int i = 0; i < H_V; ++i) {
        vert_cnt[i] = 0;
        for (int j = 0; j < H_V; ++j) {
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                if (abs((int)s_pat[plist[i]][k] - (int)s_pat[plist[j]][k]) != 1) {
                    ok = false; break;
                }
            }
            if (ok) vert_adj[i][vert_cnt[i]++] = (uint8_t)j;
        }
    }

    /* 4. first pass: count number of transitions per state */
    memset(count_arr, 0, sizeof(count_arr));
    for (uint16_t rp = 0; rp < ROWS; ++rp) {
        for (uint16_t rn = 0; rn < ROWS; ++rn) {
            uint8_t sv[4];
            for (int j = 0; j < 4; ++j)
                sv[j] = compute_s(rowv[rp][j], rowv[rp][j+1],
                                  rowv[rn][j], rowv[rn][j+1]);
            uint8_t code = encode4(sv[0], sv[1], sv[2], sv[3]);
            if (!hvalid[code]) continue;
            uint8_t vi = rev_map[code];
            ++count_arr[rp * H_V + vi];
        }
    }

    /* 5. build offset array and allocate space for next rows */
    offset_arr = (size_t*)malloc(sizeof(size_t) * STATE);
    size_t pos = 0;
    for (int i = 0; i < STATE; ++i) {
        offset_arr[i] = pos;
        pos += count_arr[i];
    }
    nxt_rows = (uint16_t*)malloc(pos * sizeof(uint16_t));

    /* 6. second pass: store next rows for each state */
    memset(count_arr, 0, sizeof(count_arr));   /* reuse as counter */
    for (uint16_t rp = 0; rp < ROWS; ++rp) {
        for (uint16_t rn = 0; rn < ROWS; ++rn) {
            uint8_t sv[4];
            for (int j = 0; j < 4; ++j)
                sv[j] = compute_s(rowv[rp][j], rowv[rp][j+1],
                                  rowv[rn][j], rowv[rn][j+1]);
            uint8_t code = encode4(sv[0], sv[1], sv[2], sv[3]);
            if (!hvalid[code]) continue;
            uint8_t vi = rev_map[code];
            size_t idx  = rp * H_V + vi;
            size_t id   = offset_arr[idx] + count_arr[idx];
            nxt_rows[id] = (uint16_t)rn;
            ++count_arr[idx];
        }
    }
}

/* ------------------------------------------------------------------ */
/* sequence function                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) { init_tables(); init = true; }

    if (n == 0) return 0;          /* offset 1 – not used by OEIS tests */

    /* clear current and next state arrays */
    memset(dp_cur, 0, sizeof(dp_cur));
    memset(dp_nxt, 0, sizeof(dp_nxt));

    /*  initial state: all pairs of first two rows */
    for (uint16_t r0 = 0; r0 < ROWS; ++r0) {
        for (uint16_t r1 = 0; r1 < ROWS; ++r1) {
            uint8_t sv[4];
            for (int j = 0; j < 4; ++j)
                sv[j] = compute_s(rowv[r0][j], rowv[r0][j+1],
                                  rowv[r1][j], rowv[r1][j+1]);
            uint8_t code = encode4(sv[0], sv[1], sv[2], sv[3]);
            if (!hvalid[code]) continue;
            uint8_t vi  = rev_map[code];
            dp_cur[r1 * H_V + vi] += (seq_t)1;
        }
    }

    /* dynamic‑program over remaining (n‑1) rows */
    for (idx_t step = 1; step < n; ++step) {
        /* propagate from current states to next states */
        for (size_t cur = 0; cur < STATE; ++cur) {
            seq_t curVal = dp_cur[cur];
            if (curVal == 0) continue;
            uint8_t rp = cur / H_V;
            uint8_t sv = cur % H_V;
            for (uint8_t v = 0; v < vert_cnt[sv]; ++v) {
                uint8_t sn = vert_adj[sv][v];
                size_t pos = rp * H_V + sn;
                size_t off = offset_arr[pos];
                size_t cnt = count_arr[pos];
                for (size_t k = 0; k < cnt; ++k) {
                    uint16_t rn = nxt_rows[off + k];
                    dp_nxt[rn * H_V + sn] += curVal;
                }
            }
        }
        /* swap current and next for next iteration */
        memcpy(dp_cur, dp_nxt, sizeof(dp_cur));
        memset(dp_nxt, 0, sizeof(dp_nxt));
    }

    /* sum all states to compute the final answer */
    seq_t result = 0;
    for (size_t i = 0; i < STATE; ++i) result += dp_cur[i];
    return result;
}

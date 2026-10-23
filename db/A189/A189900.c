/*
 * A189900  Number of nX4 binary arrays without the pattern 0 0 1 1
 *          diagonally, vertically, antidiagonally or horizontally.
 * https://oeis.org/A189900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary matrices avoiding the pattern 0 0 1
 *            1 horizontally, vertically, diagonally (↘) or anti‑diagonally
 *            (↙). For n < 4 this equals 15^n. For n ≥ 4 it is computed by a
 *            3‑row dynamic programming
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow15.h"
 
/* ---------- Precomputation tables ---------- */
 
/* For each state (a,b,c) of three consecutive rows we keep the
   allowed next rows d.  States are encoded as (a<<8)|(b<<4)|c
   (12 bits, 0–4095).  For each state we store the number of
   allowed d's, the base part (b<<8)|(c<<4), and the list of
   allowed d's.  All tables are static and filled on first call. */
static uint8_t  g_nextCount[4096];
static uint8_t  g_nextRows[4096][16];
static uint16_t g_nextBase[4096];
static int      g_tables_inited = 0;
 
static void init_tables(void)
{
    if (g_tables_inited) return;
    g_tables_inited = 1;
 
    for (uint16_t idx = 0; idx < 4096; ++idx) {
        uint16_t a = idx >> 8;          /* upper 4 bits    */
        uint16_t b = (idx >> 4) & 0xF;  /* middle 4 bits  */
        uint16_t c = idx & 0xF;         /* lower 4 bits   */
        uint8_t cnt = 0;
 
        for (uint16_t d = 0; d < 16; ++d) {
            /* horizontal restriction for the new row */
            if (d == 3) continue;      /* 0011 is forbidden */
 
            /* vertical 0 0 1 1 on any column */
            for (int j = 0; j < 4; ++j) {
                if (((a & (1u << j)) == 0) &&
                    ((b & (1u << j)) == 0) &&
                    ((c & (1u << j)) != 0) &&
                    ((d & (1u << j)) != 0)) {
                    goto next_d;
                }
            }
 
            /* main diagonal 0 0 1 1 : (a0, b1, c2, d3) */
            if (((a & 1u) == 0) &&
                ((b & 2u) == 0) &&
                ((c & 4u) != 0) &&
                ((d & 8u) != 0)) {
                goto next_d;
            }
 
            /* anti‑diagonal 0 0 1 1 : (a3, b2, c1, d0) */
            if (((a & 8u) == 0) &&
                ((b & 4u) == 0) &&
                ((c & 2u) != 0) &&
                ((d & 1u) != 0)) {
                goto next_d;
            }
 
            g_nextRows[idx][cnt++] = (uint8_t)d;
 
        next_d:
            ;
        }
        g_nextCount[idx] = cnt;
        g_nextBase[idx] = (b << 8) | (c << 4);
    }
}
 

 
/* ---------- DP tables for states of three rows ---------- */
static seq_t dp0[4096];
static seq_t dp1[4096];
static seq_t *dp_prev = dp0;
static seq_t *dp_cur  = dp1;
 
/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* for small n we avoid DP completely */
    if (n == 0) return 0;               /* not defined for OEIS, harmless */
    if (n < 4) return pow15(n);
 
    init_tables();                     /* build transitions once */
 
    /* initialise DP: all triples that do not contain 0011 horizontally */
    for (uint32_t i = 0; i < 4096; ++i) {
        dp_prev[i] = dp_cur[i] = 0;
    }
    for (uint16_t idx = 0; idx < 4096; ++idx) {
        uint16_t a_ = idx >> 8;
        uint16_t b_ = (idx >> 4) & 0xF;
        uint16_t c_ = idx & 0xF;
        if (a_ != 3 && b_ != 3 && c_ != 3) dp_prev[idx] = 1;
    }
 
    /* DP for rows 4 … n */
    for (idx_t row = 4; row <= n; ++row) {
        /* clear current table */
        for (uint32_t i = 0; i < 4096; ++i) dp_cur[i] = 0;
 
        /* transition */
        for (uint16_t idx = 0; idx < 4096; ++idx) {
            seq_t val = dp_prev[idx];
            if (val == 0) continue;
            uint8_t   cnt   = g_nextCount[idx];
            uint16_t  base  = g_nextBase[idx];
            const uint8_t *list = g_nextRows[idx];
            for (uint8_t k = 0; k < cnt; ++k) {
                uint8_t d = list[k];
                uint16_t newIdx = base | d;  /* (b,c,d) */
                dp_cur[newIdx] += val;
            }
        }
        /* swap tables for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }
 
    /* sum over all final states */
    seq_t total = 0;
    for (uint32_t i = 0; i < 4096; ++i) total += dp_prev[i];
    return total;
}

/*
 * A207684  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207684: Number of 4 × n 0..1 arrays avoiding 000, 101 horizontally
 * and 001, 111 vertically.
 *
 * The algorithm counts valid arrays by dynamic programming over columns.
 * A state keeps the last two columns (8 bits: two 4‑bit patterns).  From
 * any state we try all 9 admissible columns (those with no vertical 001
 * or 111).  A transition is allowed if adding the new column does not
 * create a forbidden horizontal triple in any row.  The number of ways
 * to reach each state after m columns is stored in dp_cur.  Summing
 * over all states yields a(m).  The table is built up to the requested
 * n, and previously computed terms are cached for repeated or out‑of‑
 * order queries.
 *
 * Complexity:  O(n * 256 * 9) time, O(256) memory (apart from cache).
 * Works for any seq_t (up to 128 bits) and any idx_t (uint64_t). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* dynamic programming arrays (static global to survive multiple calls) */
static bool initialized = false;
static int allowed[9];
static int allowed_cnt = 0;
static bool trans_valid[256][9];
static uint8_t trans_state[256][9];
static seq_t dp_cur[256];
static seq_t dp_next[256];
static seq_t *cache = NULL;          /* cache[0..max_computed] */
static size_t cache_cap = 0;         /* capacity of cache */
static size_t cache_len = 0;         /* highest index computed (1‑based) */
static size_t dp_len = 2;            /* columns processed (initially 2) */

seq_t a(idx_t n)
{
    /*---------------------------------------------------------------*/
    /* initialization (runs once)                                    */
    /*---------------------------------------------------------------*/
    if (!initialized) {
        /* admissible 4‑bit columns (no vertical 001 or 111) */
        for (int pat = 0; pat < 16; ++pat) {
            bool ok = true;
            int r012 = (pat >> 0) & 7;  /* bits 0,1,2 */
            if (r012 == 0b001 || r012 == 0b111) ok = false;
            int r123 = (pat >> 1) & 7;  /* bits 1,2,3 */
            if (r123 == 0b001 || r123 == 0b111) ok = false;
            if (ok) allowed[allowed_cnt++] = pat;   /* should become 9 */
        }

        /* build transition tables */
        for (int s = 0; s < 256; ++s) {
            for (int ci = 0; ci < allowed_cnt; ++ci) {
                int c = allowed[ci];
                bool ok = true;
                for (int r = 0; r < 4; ++r) {
                    int p2 = (s >> (2*r+1)) & 1;   /* previous column (distance 2) */
                    int p1 = (s >> (2*r)) & 1;     /* column before that */
                    int cc = (c >> r) & 1;
                    if ((p2 == 0 && p1 == 0 && cc == 0) ||
                        (p2 == 1 && p1 == 0 && cc == 1)) {
                        ok = false;
                        break;
                    }
                }
                trans_valid[s][ci] = ok;
                if (!ok) {
                    trans_state[s][ci] = 0;
                    continue;
                }
                /* new state: column c becomes previous1, old previous1 becomes previous2 */
                int ns = 0;
                for (int r = 0; r < 4; ++r) {
                    int cc = (c >> r) & 1;
                    int p1 = (s >> (2*r)) & 1;
                    ns |= (cc << (2*r));
                    ns |= (p1 << (2*r+1));
                }
                trans_state[s][ci] = (uint8_t)ns;
            }
        }

        /* initialise DP for two columns (all pairs of allowed columns) */
        for (int i = 0; i < 256; ++i) dp_cur[i] = 0;
        for (int c1 = 0; c1 < allowed_cnt; ++c1) {
            int col1 = allowed[c1];
            for (int c2 = 0; c2 < allowed_cnt; ++c2) {
                int col2 = allowed[c2];
                int state = 0;
                for (int r = 0; r < 4; ++r) {
                    int b1 = (col2 >> r) & 1;   /* most recent column */
                    int b2 = (col1 >> r) & 1;   /* previous column */
                    state |= (b1 << (2*r));
                    state |= (b2 << (2*r+1));
                }
                dp_cur[state] += (seq_t)1;
            }
        }

        /* cache first two terms */
        cache_cap = 20;  /* small initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[1] = (seq_t)9;   /* a(1) = 9 admissible columns */
        cache[2] = (seq_t)81;  /* a(2) = 9^2 */
        cache_len = 2;

        initialized = true;
    }

    /*---------------------------------------------------------------*/
    /* handle very small n                                             */
    /*---------------------------------------------------------------*/
    if (n == 0) return 0;  /* undefined, but return 0 as placeholder */
    if ((size_t)n <= cache_len) return cache[(size_t)n];

    /*---------------------------------------------------------------*/
    /* grow cache: compute up to n                                     */
    /*---------------------------------------------------------------*/
    /* ensure cache capacity */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        for (size_t i = 0; i <= cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = needed;
    }

    /* compute DP step by step from current dp_len up to n */
    while (dp_len < n) {
        /* transition to next column */
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            for (int ci = 0; ci < allowed_cnt; ++ci) {
                if (!trans_valid[s][ci]) continue;
                uint8_t ns = trans_state[s][ci];
                dp_next[ns] += val;
            }
        }
        /* copy dp_next to dp_cur for next iteration */
        for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];

        /* sum all states to get a(dp_len + 1) */
        seq_t total = 0;
        for (int i = 0; i < 256; ++i) total += dp_cur[i];
        dp_len++;
        cache[dp_len] = total;
        if (dp_len > cache_len) cache_len = dp_len;
    }

    return cache[(size_t)n];
}

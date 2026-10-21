/*
 * A207842  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‑programming computation of OEIS A207842.
   The problem counts n×5 binary arrays avoiding the patterns:
   horizontally 000 and 001, vertically 000 and 111.
   The table of allowed 5‑bit rows is of size 32; we keep for each
   pair of consecutive rows the list of possible next rows.  The
   DP state records the last two rows, using an index (prev<<5)|curr.
   For n=1 we count the allowed rows; for n=2 we count all ordered
   pairs of allowed rows; for n≥3 we iterate adding one row at a time
   while respecting the vertical constraint.  Results are memoised
   in a growing static array.  The function tolerates any build
   supplying seq_t, idx_t, and any required headers. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* size of a row (five columns) */
#define ROW_BITS 5
#define ROW_MASK 31          /* 5‑bit mask: 0…31 */

/* DP state width: (prev<<5)|curr   -> 32*32 = 1024 */
#define STATE_CNT (ROW_MASK+1)*(ROW_MASK+1)

/* static tables – built once */
static bool ok_row[ROW_MASK+1] = {0};
static int  next_cnt[STATE_CNT] = {0};
static uint8_t next_mask[STATE_CNT][ROW_MASK+1] = {0};
static bool tables_inited = false;

/* helper to build the tables */
static void init_tables(void)
{
    if (tables_inited) return;
    /* which 5‑bit rows are allowed horizontally? */
    for (int m = 0; m <= ROW_MASK; ++m) {
        bool ok = true;
        for (int s = 0; s <= ROW_BITS-3; ++s) {
            int seg = (m >> s) & 0x7;          /* 3‑bit window */
            if (seg == 0 || seg == 1) {        /* 000 or 001 */
                ok = false;
                break;
            }
        }
        ok_row[m] = ok;
    }

    /* pre‑compute transitions for each pair (prev,curr) to next */
    for (int prev = 0; prev <= ROW_MASK; ++prev) {
        for (int curr = 0; curr <= ROW_MASK; ++curr) {
            int idx = (prev << ROW_BITS) | curr;
            int count = 0;
            for (int nxt = 0; nxt <= ROW_MASK; ++nxt) {
                if (!ok_row[nxt]) continue;
                bool good = true;
                for (int col = 0; col < ROW_BITS; ++col) {
                    int bprev = (prev >> col) & 1;
                    int bcurr = (curr >> col) & 1;
                    int bnxt  = (nxt  >> col) & 1;
                    if (bprev == bcurr && bcurr == bnxt) {
                        good = false;
                        break;
                    }
                }
                if (good) {
                    next_mask[idx][count++] = (uint8_t)nxt;
                }
            }
            next_cnt[idx] = count;
        }
    }
    tables_inited = true;
}

/* dynamic cache for computed a(n) */
static seq_t *a_cache = NULL;
static size_t a_cap = 0;
static idx_t a_done = 0;          /* largest n cached (0 means none) */

/* ensure the cache can hold at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if ((idx_t)a_cap > n) return;
    size_t new_cap = a_cap > 0 ? a_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failure */
    a_cache = tmp;
    a_cap = new_cap;
}

/* DP arrays for current and next layer */
static seq_t dp_cur[STATE_CNT] = {0};
static seq_t dp_next[STATE_CNT] = {0};

/* main function implementing the sequence */
seq_t a(idx_t n)
{
    init_tables();                     /* build static tables once */

    if (n < 1) return 0;               /* out of range */

    ensure_cache(n);                   /* grow cache if needed */

    /* if we already have the requested value, return it */
    if (a_done >= n) return a_cache[n];

    /* first compute base cases if not done yet */
    if (a_done < 1) {
        seq_t sum = 0;
        for (int m = 0; m <= ROW_MASK; ++m)
            if (ok_row[m]) sum += 1;
        a_cache[1] = sum;
        a_done = 1;
    }
    if (a_done < 2 && n >= 2) {
        seq_t sum2 = 0;
        for (int prev = 0; prev <= ROW_MASK; ++prev)
            if (ok_row[prev])
                for (int curr = 0; curr <= ROW_MASK; ++curr)
                    if (ok_row[curr])
                        sum2 += 1;      /* all ordered pairs allowed */
        a_cache[2] = sum2;
        a_done = 2;
    }

    /* initialise dp_cur for length 2 if needed */
    if (a_done == 2 && n > 2) {
        memset(dp_cur, 0, sizeof(dp_cur));
        for (int prev = 0; prev <= ROW_MASK; ++prev)
            if (ok_row[prev])
                for (int curr = 0; curr <= ROW_MASK; ++curr)
                    if (ok_row[curr])
                        dp_cur[(prev << ROW_BITS) | curr] = 1;
    }
    idx_t cur_len = a_done;            /* current number of rows in dp_cur */

    /* iterate until we reach n */
    for (idx_t len = cur_len + 1; len <= n; ++len) {
        memset(dp_next, 0, sizeof(dp_next));

        for (int idx = 0; idx < STATE_CNT; ++idx) {
            seq_t val = dp_cur[idx];
            if (!val) continue;
            int prev = idx >> ROW_BITS;
            int curr = idx & ROW_MASK;
            int cnt  = next_cnt[idx];
            for (int k = 0; k < cnt; ++k) {
                int nxt = next_mask[idx][k];
                int nidx = (curr << ROW_BITS) | nxt;
                dp_next[nidx] += val;
            }
        }

        /* compute total for this length */
        seq_t sum = 0;
        for (int i = 0; i < STATE_CNT; ++i) sum += dp_next[i];

        a_cache[len] = sum;
        a_done = len;

        /* swap arrays for next iteration */
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    return a_cache[n];
}

/*
 * A207860  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 5×n binary arrays without horizontal 000 or 010
 *                and without vertical 010 or 101.
 *              a(1) = number of valid columns, a(2) = a(1)^2, and
 *              the recurrence is computed by dynamic programming over
 *              states consisting of the last two columns.
 *
 * Each state is a pair of columns (x,y).  Transition to a new column z
 * is allowed iff for every row r (0…4) the horizontal 3‑tuple
 * (bit_r(x), bit_r(y), bit_r(z)) is neither 000 nor 010.  The vertical
 * restriction is checked when enumerating the columns themselves.
 *
 * This implementation memoizes results and runs in O(n·S) where
 * S = (#valid columns)^2 ≤ 256, easily fast enough for n ≤ 10 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

/* ----- core routine ----------------------------------------------------- */
static seq_t a_static(idx_t const n)
{
    /* constants */
    enum { R = 5, C = 1u << R, ST = C * C };    /* 5 rows, 5-bit columns, state count */

    /* ---------- static state ------------------------------------------ */
    static bool init = false;
    static bool valid_col[C];
    static int  Vcnt;                     /* number of valid columns      */
    static int  nextcnt[ST];              /* transitions per state        */
    static int  nextcol[ST][C];           /* dest column for each trans   */
    static seq_t dp0[ST], dp1[ST];        /* DP layers for two columns    */
    static seq_t *cur = dp0, *nxt = dp1;  /* current / next state arrays  */
    static seq_t *seq = NULL;             /* cached sequence          */
    static idx_t  seq_len = 0;            /* number of cached terms    */
    static idx_t  seq_cap = 0;            /* capacity of seq array    */

    /* ---------- initialization ---------------------------------------- */
    if (!init) {
        /* ---- enumerate valid columns (vertical restriction) --------- */
        for (int col = 0; col < C; ++col) {
            bool good = true;
            for (int i = 0; i < 3; ++i) {          /* triples of rows */
                int a = (col >> i)     & 1;
                int b = (col >> (i+1)) & 1;
                int c = (col >> (i+2)) & 1;
                if ((a==0 && b==1 && c==0) || (a==1 && b==0 && c==1)) {
                    good = false;
                    break;
                }
            }
            valid_col[col] = good;
        }
        Vcnt = 0;
        for (int col = 0; col < C; ++col)
            if (valid_col[col]) ++Vcnt;

        /* ---- pre‑compute transitions for all pairs (x,y) --------- */
        memset(nextcnt, 0, sizeof(nextcnt));
        for (int x = 0; x < C; ++x) if (valid_col[x]) {
            for (int y = 0; y < C; ++y) if (valid_col[y]) {
                int idx = (x << R) | y;              /* state key */
                for (int z = 0; z < C; ++z) if (valid_col[z]) {
                    /* check horizontal 3‑tuple for every row */
                    bool ok = true;
                    for (int r = 0; r < R; ++r) {
                        int pat  = (((x >> r) & 1) << 2)
                                  | (((y >> r) & 1) << 1)
                                  | ((z >> r) & 1);
                        if (pat == 0 || pat == 2) {   /* 000 or 010 */
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;
                    int j = nextcnt[idx];
                    nextcol[idx][j] = z;
                    ++nextcnt[idx];
                }
            }
        }

        /* ---- initial DP for n = 2 (no horizontal restriction) ----- */
        memset(dp0, 0, sizeof(dp0));
        for (int x = 0; x < C; ++x) if (valid_col[x]) {
            for (int y = 0; y < C; ++y) if (valid_col[y]) {
                dp0[(x << R) | y] = 1;
            }
        }

        /* ---- cache first two terms -------------------------------- */
        seq_len = 2;
        seq_cap = 2;
        seq = seq_alloc_new_array(seq_cap);
        seq[0] = (seq_t)Vcnt;                      /* a(1) */
        seq[1] = (seq_t)Vcnt * (seq_t)Vcnt;         /* a(2) */

        init = true;
    }

    /* ---- return cached value if available ------------------------ */
    if (n <= seq_len)
        return seq[n - 1];

    /* ---- extend the cache up to required index -------------------- */
    while (seq_len < n) {
        /* reset next layer */
        memset(nxt, 0, sizeof(dp1));

        /* propagate all states from cur to nxt via stored transitions */
        for (int idx = 0; idx < ST; ++idx) {
            seq_t ways = cur[idx];
            if (!ways) continue;
            for (int k = 0; k < nextcnt[idx]; ++k) {
                int z   = nextcol[idx][k];
                int idx2 = ((idx & (C-1)) << R) | z;     /* state (y,z) */
                nxt[idx2] += ways;
            }
        }

        /* sum all ways to obtain a(n) */
        seq_t term = 0;
        for (int i = 0; i < ST; ++i)
            term += nxt[i];

        /* extend the sequence array if necessary */
        if (seq_len + 1 > seq_cap) {
            idx_t new_cap = seq_cap * 2;
            if (new_cap < seq_len + 1) new_cap = seq_len + 1;
            seq_t *new_seq = seq_alloc_new_array(new_cap);
            memcpy(new_seq, seq, seq_len * sizeof(seq_t));
            seq_free(seq);
            seq = new_seq;
            seq_cap = new_cap;
        }
        seq[seq_len] = term;
        ++seq_len;

        /* swap dp layers for next iteration */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    return seq[n - 1];
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) { return a_static(n); }

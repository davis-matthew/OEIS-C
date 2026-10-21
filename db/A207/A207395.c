/*
 * A207395  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207395: Number of 6×n binary arrays avoiding  
   horizontally 0 0 1 and 0 1 0, vertically 0 0 1 and 1 1 0.  
   The algorithm uses a transfer‑matrix on the set of allowed 6‑bit
   columns (12 of them).  A state is a pair of consecutive columns
   and transitions correspond to adding a new column that satisfies
   the horizontal constraints.  The DP runs in O(n·|states|·|cols|) ≈
   O(n) with |states|=12²=144. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* One‑time initialisation of the column list and state transitions */
    /* ------------------------------------------------------------------ */
    static bool init = false;
    static unsigned col_bits[12];            /* valid 6‑bit columns      */
    static unsigned char col_cnt;            /* =12                       */
    static unsigned short next_state[144][12];
    static unsigned char next_cnt[144];

    if (!init) {
        /* 1. Find all 12 columns that avoid 001 and 110 vertically          */
        unsigned temp[12];
        unsigned cnt = 0;
        for (unsigned bits = 0; bits < 64; ++bits) {
            bool ok = true;
            for (unsigned r = 0; r + 2 < 6 && ok; ++r) {
                unsigned b0 = (bits >> r)     & 1;
                unsigned b1 = (bits >> (r+1)) & 1;
                unsigned b2 = (bits >> (r+2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 1 && b1 == 1 && b2 == 0))
                    ok = false;
            }
            if (ok) temp[cnt++] = bits;
        }
        col_cnt = cnt;                      /* should be 12          */
        for (unsigned i = 0; i < col_cnt; ++i) col_bits[i] = temp[i];

        /* 2. Build transition lists for the 144 states (prev2,prev1)       */
        for (unsigned p2 = 0; p2 < col_cnt; ++p2) {
            for (unsigned p1 = 0; p1 < col_cnt; ++p1) {
                unsigned state = p2 * col_cnt + p1;
                unsigned char nxt = 0;
                for (unsigned nc = 0; nc < col_cnt; ++nc) {
                    bool ok = true;
                    for (unsigned r = 0; r < 6 && ok; ++r) {
                        unsigned b2 = (col_bits[p2] >> r) & 1;
                        unsigned b1 = (col_bits[p1] >> r) & 1;
                        unsigned b0 = (col_bits[nc] >> r) & 1;
                        if ((b2 == 0 && b1 == 0 && b0 == 1) ||
                            (b2 == 0 && b1 == 1 && b0 == 0))
                            ok = false;
                    }
                    if (ok) {
                        unsigned next = p1 * col_cnt + nc;   /* new state */
                        next_state[state][nxt++] = next;
                    }
                }
                next_cnt[state] = nxt;
            }
        }
        init = true;
    }

    /* ------------------------------------------------------------------ */
    /* Quick answers for n = 1 and n = 2                                    */
    /* ------------------------------------------------------------------ */
    if (n == 1) return (seq_t)col_cnt;                 /* 12              */
    if (n == 2) return (seq_t)(col_cnt * col_cnt);     /* 144             */

    /* ------------------------------------------------------------------ */
    /* DP over columns (starting after the second column)                  */
    /* ------------------------------------------------------------------ */
    seq_t dp_cur[144] = {0};
    seq_t dp_next[144] = {0};

    /* initialise after two columns: every ordered pair of allowed columns
       yields one distinct state                                         */
    for (unsigned p2 = 0; p2 < col_cnt; ++p2)
        for (unsigned p1 = 0; p1 < col_cnt; ++p1)
            dp_cur[p2 * col_cnt + p1] = 1;

    /* iterate over columns 3 … n                                            */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero next DP table */
        for (unsigned i = 0; i < 144; ++i) dp_next[i] = 0;

        /* propagate transitions */
        for (unsigned state = 0; state < 144; ++state) {
            seq_t cur = dp_cur[state];
            if (!cur) continue;
            unsigned char m = next_cnt[state];
            unsigned short *list = next_state[state];
            for (unsigned t = 0; t < m; ++t)
                dp_next[list[t]] += cur;
        }

        /* swap current and next */
        for (unsigned i = 0; i < 144; ++i) dp_cur[i] = dp_next[i];
    }

    /* ------------------------------------------------------------------ */
    /* The answer is the sum over all states after n columns               */
    /* ------------------------------------------------------------------ */
    seq_t sum = 0;
    for (unsigned i = 0; i < 144; ++i) sum += dp_cur[i];
    return sum;
}

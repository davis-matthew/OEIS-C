/*
 * A208120  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A208120: number of 5×n 0..1 arrays avoiding the
   horizontal patterns 0 0 0 and 0 1 0, and the vertical patterns
   0 0 1 and 0 1 1.  The value is obtained by dynamic programming. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* pre‑compute allowed column patterns and transitions */
    static bool init = false;
    static bool col_allowed[32];
    static bool trans[32][32][32];
    static seq_t num_allowed = 0;

    if (!init) {
        /* Count column patterns satisfying vertical constraints */
        for (idx_t s = 0; s < 32; ++s) {        /* 5 rows → 5 bits */
            bool ok = true;
            for (idx_t r = 0; r < 3; ++r) {
                int b0 = (s >> r)   & 1;
                int b1 = (s >> (r+1)) & 1;
                int b2 = (s >> (r+2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 1)) {
                    ok = false;
                    break;
                }
            }
            col_allowed[s] = ok;
            if (ok) ++num_allowed;
        }

        /* Pre‑compute transition validity among triples of columns */
        for (idx_t p = 0; p < 32; ++p) {
            if (!col_allowed[p]) continue;
            for (idx_t q = 0; q < 32; ++q) {
                if (!col_allowed[q]) continue;
                for (idx_t r = 0; r < 32; ++r) {
                    if (!col_allowed[r]) continue;
                    bool ok = true;
                    for (idx_t row = 0; row < 5; ++row) {
                        int c0 = (p >> row) & 1;
                        int c1 = (q >> row) & 1;
                        int c2 = (r >> row) & 1;
                        if ((c0 == 0 && c1 == 0 && c2 == 0) ||
                            (c0 == 0 && c1 == 1 && c2 == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    trans[p][q][r] = ok;
                }
            }
        }
        init = true;
    }

    if (n == 0) return 0;          /* offset 1 as per OEIS */
    if (n == 1) return num_allowed;

    /* DP: states are pairs of last two columns */
    static seq_t cur[32][32];
    static seq_t nxt[32][32];

    /* initialise for step 2: all allowed pairs have 1 way */
    for (idx_t p = 0; p < 32; ++p)
        if (col_allowed[p])
            for (idx_t q = 0; q < 32; ++q)
                if (col_allowed[q])
                    cur[p][q] = 1;

    for (idx_t step = 3; step <= n; ++step) {
        /* reset nxt */
        for (idx_t i = 0; i < 32; ++i)
            for (idx_t j = 0; j < 32; ++j)
                nxt[i][j] = 0;

        for (idx_t p = 0; p < 32; ++p) {
            if (!col_allowed[p]) continue;
            for (idx_t q = 0; q < 32; ++q) {
                if (!col_allowed[q]) continue;
                seq_t ways = cur[p][q];
                if (ways == 0) continue;
                for (idx_t r = 0; r < 32; ++r) {
                    if (trans[p][q][r]) {
                        nxt[q][r] += ways;
                    }
                }
            }
        }
        for (idx_t i = 0; i < 32; ++i)
            for (idx_t j = 0; j < 32; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* sum all states to get a(n) */
    seq_t total = 0;
    for (idx_t p = 0; p < 32; ++p)
        if (col_allowed[p])
            for (idx_t q = 0; q < 32; ++q)
                if (col_allowed[q])
                    total += cur[p][q];
    return total;
}

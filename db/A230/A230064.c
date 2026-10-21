/*
 * A230064  Number of nX4 0..2 arrays x(i,j) with each element diagonally or
 *          antidiagonally next to at least one element with value 2-x(i,j).
 * https://oeis.org/A230064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A230064
 * Number of n×4 arrays x(i,j) over {0,1,2} such that each element
 * has a diagonal or antidiagonal neighbour with value 2‑x(i,j).
 *
 * Dynamic programming over rows with 4‑bit masks for unsatisfied cells.
 * Complexity:  O(n·81·81·16) time, negligible memory.
 * Works for any seq_t/idx_t provided by the build. */
#include <stdint.h>
#include <stdbool.h>

/* forward declarations for the build environment */
/* idx_t and seq_t are supplied by the build system */

/* local helper structure for pre‑computed transitions */
struct transition {
    unsigned char b;   /* next row index */
    unsigned char m2;  /* resulting mask for next row */
};

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Pre‑computation – performed only once                          */
    /* ------------------------------------------------------------------ */
    static bool init_done = false;

    /* All 3³⁴ patterns (81) for a 4‑column row */
    static unsigned char row_val[81][4];
    /* For a pair (a,b) bitmask of cells in a satisfied by b */
    static unsigned char sat_ab[81][81];
    /* Transition lists for every (a,mask) pair */
    static struct transition trans[81][16][81];
    static unsigned char trans_len[81][16];

    if (!init_done) {
        /* Build row patterns (base‑3 representation) */
        for (int r = 0; r < 81; ++r) {
            int tmp = r;
            for (int c = 0; c < 4; ++c) {
                row_val[r][c] = (unsigned char)(tmp % 3);
                tmp /= 3;
            }
        }

        /* Pre–compute satisfy masks sat_ab[a][b] */
        for (int a = 0; a < 81; ++a) {
            for (int b = 0; b < 81; ++b) {
                unsigned char mask = 0;
                for (int j = 0; j < 4; ++j) {
                    unsigned char target = 2 - row_val[a][j];
                    bool found = false;
                    if (j > 0 && row_val[b][j - 1] == target) found = true;
                    if (!found && j < 3 && row_val[b][j + 1] == target) found = true;
                    if (found) mask |= (1U << j);
                }
                sat_ab[a][b] = mask;
            }
        }

        /* Pre–compute transition lists */
        for (int a = 0; a < 81; ++a) {
            for (int m = 0; m < 16; ++m) {
                unsigned char *plen = &trans_len[a][m];
                *plen = 0;
                for (int b = 0; b < 81; ++b) {
                    if (m && (~sat_ab[a][b] & m)) continue; /* some unsatisfied bit not covered */
                    unsigned char m2 = (~sat_ab[b][a]) & 15U; /* bits still unsatisfied in b */
                    struct transition *t = &trans[a][m][*plen];
                    t->b  = (unsigned char)b;
                    t->m2 = m2;
                    (*plen)++;
                }
            }
        }

        init_done = true;
    }

    /* ------------------------------------------------------------------ */
    /* DP over rows                                                     */
    /* ------------------------------------------------------------------ */
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 */

    static seq_t dp[81][16];
    static seq_t dp_next[81][16];
    /* clear dp */
    for (int a = 0; a < 81; ++a)
        for (int m = 0; m < 16; ++m) dp[a][m] = (seq_t)0;

    /* initial state: only the first row, all cells unsatisfied (mask 15) */
    for (int a = 0; a < 81; ++a) dp[a][15] = (seq_t)1;

    /* process n-1 transitions */
    for (idx_t step = 1; step < n; ++step) {
        /* clear dp_next */
        for (int a = 0; a < 81; ++a)
            for (int m = 0; m < 16; ++m) dp_next[a][m] = (seq_t)0;

        for (int a = 0; a < 81; ++a) {
            for (int m = 0; m < 16; ++m) {
                seq_t val = dp[a][m];
                if (val == (seq_t)0) continue;
                unsigned char *plen = &trans_len[a][m];
                for (int k = 0; k < *plen; ++k) {
                    unsigned char b  = trans[a][m][k].b;
                    unsigned char m2 = trans[a][m][k].m2;
                    dp_next[b][m2] += val;
                }
            }
        }

        /* swap dp and dp_next for next step */
        for (int a = 0; a < 81; ++a)
            for (int m = 0; m < 16; ++m) dp[a][m] = dp_next[a][m];
    }

    /* Sum over final states where all cells are satisfied (mask 0) */
    seq_t total = (seq_t)0;
    for (int a = 0; a < 81; ++a)
        total += dp[a][0];

    return total;
}

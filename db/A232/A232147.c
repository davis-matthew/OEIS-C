/*
 * A232147  Number of n X 4 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally, diagonally or antidiagonally.
 * https://oeis.org/A232147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Dynamic programming over rows.  There are 81 possible
   patterns for a row of 4 cells (values 0,1,2).  For each pattern
   we keep a 4‑bit mask of positions of 0 or 1 that still need a
   neighboring 1 (for a 0) or 2 (for a 1).  A transition from a row
   with pattern P and need mask N to a row with pattern Q is
   allowed only if every position in N is satisfied by a diagonal
   neighbour in Q.  The new need mask for Q is computed from
   horizontal neighbours in Q and diagonal neighbours from P.
   The transition data for every pair of patterns is pre‑computed
   once per call.  After processing all n rows the answer is the
   sum of counts of states whose need mask is zero.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Constants */
    const size_t NUM_PAT = 81;   /* 3⁴ */
    const size_t MASKS   = 16;   /* 4 bits */

    /* Decode all 81 patterns into a[pat][col] (col 0..3) */
    int pat[NUM_PAT][4];
    for (idx_t id = 0; id < NUM_PAT; ++id) {
        idx_t x = id;
        for (int c = 3; c >= 0; --c) {
            pat[id][c] = x % 3;
            x /= 3;
        }
    }

    /* Pre‑compute transition tables:
         unsat[p][q] – bits of columns in row p that are not
                      satisfied by diagonal neighbours in row q
         newneed[p][q] – need mask for row q after reading row p  */
    uint8_t unsat[NUM_PAT][NUM_PAT];
    uint8_t newneed[NUM_PAT][NUM_PAT];
    for (int p = 0; p < NUM_PAT; ++p) {
        for (int q = 0; q < NUM_PAT; ++q) {
            uint8_t u = 0, n = 0;
            for (int c = 0; c < 4; ++c) {
                int pv = pat[p][c];
                int qv = pat[q][c];

                if (pv == 0 || pv == 1) {
                    int req = (pv == 0) ? 1 : 2;
                    bool sat = false;
                    if (c > 0 && pat[q][c-1] == req) sat = true;
                    if (!sat && c < 3 && pat[q][c+1] == req) sat = true;
                    if (!sat) u |= (1 << c);
                }

                if (qv == 0 || qv == 1) {
                    int req = (qv == 0) ? 1 : 2;
                    bool sat = false;
                    if (c > 0 && pat[q][c-1] == req) sat = true;
                    if (!sat && c < 3 && pat[q][c+1] == req) sat = true;
                    if (!sat && c > 0 && pat[p][c-1] == req) sat = true;
                    if (!sat && c < 3 && pat[p][c+1] == req) sat = true;
                    if (!sat) n |= (1 << c);
                }
            }
            unsat[p][q] = u;
            newneed[p][q] = n;
        }
    }

    /* DP arrays */
    seq_t dp_cur[NUM_PAT][MASKS];
    seq_t dp_next[NUM_PAT][MASKS];
    for (int i = 0; i < NUM_PAT; ++i)
        for (int j = 0; j < MASKS; ++j)
            dp_cur[i][j] = 0;

    /* Initialise with first row */
    for (int p = 0; p < NUM_PAT; ++p) {
        uint8_t need = 0;
        for (int c = 0; c < 4; ++c) {
            int cv = pat[p][c];
            if (cv == 0 || cv == 1) {
                int req = (cv == 0) ? 1 : 2;
                bool sat = false;
                if (c > 0 && pat[p][c-1] == req) sat = true;
                if (!sat && c < 3 && pat[p][c+1] == req) sat = true;
                if (!sat) need |= (1 << c);
            }
        }
        dp_cur[p][need] = 1;
    }

    /* Process remaining rows */
    seq_t zero = 0;
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next table */
        for (int i = 0; i < NUM_PAT; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < NUM_PAT; ++prev) {
            for (int need = 0; need < MASKS; ++need) {
                if (dp_cur[prev][need] == zero) continue;
                seq_t val = dp_cur[prev][need];
                for (int curr = 0; curr < NUM_PAT; ++curr) {
                    /* check that all pending requests from prev are satisfied */
                    if (need & unsat[prev][curr]) continue;
                    uint8_t new_need = newneed[prev][curr];
                    dp_next[curr][new_need] += val;
                }
            }
        }

        /* swap cur and next */
        for (int i = 0; i < NUM_PAT; ++i)
            for (int j = 0; j < MASKS; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Sum all states with no pending requirements */
    seq_t total = 0;
    for (int p = 0; p < NUM_PAT; ++p)
        total += dp_cur[p][0];

    return total;
}

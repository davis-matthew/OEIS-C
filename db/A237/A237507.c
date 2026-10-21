/*
 * A237507  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock differing from its
 *          horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   absdiff, encodeM
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237507: Number of (n+1)×5 arrays with entries 0..2 such that
 * for every 2×2 subblock the value M = max + upper_median – min differs
 * from each of its horizontal and vertical neighbours by exactly one.
 *
 * The implementation uses a dynamic‑programming algorithm over rows.
 * A state consists of the last row and the 4×1 vector of M‑values computed
 * for the most recent pair of rows.  Transition to the next row requires
 * the horizontal rule within the new pair and the vertical rule between
 * the two adjacent pairs of rows.  All counts are performed in seq_t.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/absdiff.h"
#include "../utils/encodeM.h"

#define NUM_ROWS   243        /* 3^5 possible rows (base‑3 digits)   */
#define M_CODES    625        /* 5^4 possible 4‑tuple M values      */
#define STATE_SZ   (NUM_ROWS * M_CODES)

/* row_val[r][c] : base‑3 digit at position c of row r            */
static uint8_t  row_val[NUM_ROWS][5];
/* pair_m[prev][nxt][4] : the 4 M‑values for the pair (prev,nxt)   */
static uint8_t  pair_m[NUM_ROWS * NUM_ROWS][4];
/* pair_enc[prev][nxt] : 5‑base encoding of the 4 M‑values         */
static uint16_t pair_enc[NUM_ROWS * NUM_ROWS];
/* horiz_next[prev][k] : list of rows nxt such that pair (prev,nxt)
 * satisfies the horizontal rule.  horiz_cnt[prev] gives its length. */
static uint8_t  horiz_next[NUM_ROWS][NUM_ROWS];
static uint8_t  horiz_cnt[NUM_ROWS];
/* decode_M[code][i] : decode 5‑base encoding back to 4 M‑values      */
static uint8_t  decode_M[M_CODES][4];

/* flag signalling that the tables have already been built */
static bool init_done = false;

/* ------------------------------------------------------------------ */
/* Compute absolute difference of two ints (no stdlib call)            */

/* ------------------------------------------------------------------ */
/* Build all helper tables needed for the DP.  This routine is called
 * only once, and the tables are cached for all subsequent calls.     */
static void build_tables(void)
{
    if (init_done) return;

    /* 1) Convert row indices to arrays of 5 base‑3 digits.              */
    for (uint32_t r = 0; r < NUM_ROWS; ++r) {
        uint32_t x = r;
        for (int c = 0; c < 5; ++c) {
            row_val[r][c] = x % 3;
            x /= 3;
        }
    }

    /* 2) Decode M‑codes back to individual values.                    */
    for (int code = 0; code < (int)M_CODES; ++code) {
        int tmp = code;
        for (int j = 0; j < 4; ++j) {
            decode_M[code][j] = tmp % 5;
            tmp /= 5;
        }
    }

    /* 3) Precompute M values for every 2×2 block (0..3) for all 4
     *    possible arrangements with cells 0..2.                           */
    uint8_t m_tab[81];               /* 3^4 possible (a,b,c,d) */
    for (uint8_t a = 0; a < 3; ++a)
        for (uint8_t b = 0; b < 3; ++b)
            for (uint8_t c = 0; c < 3; ++c)
                for (uint8_t d = 0; d < 3; ++d) {
                    uint8_t arr[4] = {a, b, c, d};
                    /* sort ascending in place */
                    for (int i = 0; i < 4; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (arr[i] > arr[j]) {
                                uint8_t t = arr[i];
                                arr[i] = arr[j];
                                arr[j] = t;
                            }
                    uint8_t maxv = arr[3];
                    uint8_t lowv = arr[0];
                    uint8_t midv  = arr[2];
                    uint8_t idx   = a * 27 + b * 9 + c * 3 + d;
                    m_tab[idx] = maxv + midv - lowv;
                }

    /* 4) For each ordered pair of rows compute the 4 M‑values and
     *    pre‑store the horizontal compatibility list.                */
    for (int prev = 0; prev < NUM_ROWS; ++prev) {
        for (int nxt = 0; nxt < NUM_ROWS; ++nxt) {
            uint8_t *mvec = pair_m[prev * NUM_ROWS + nxt];
            for (int j = 0; j < 4; ++j) {
                uint8_t a = row_val[prev][j];
                uint8_t b = row_val[prev][j + 1];
                uint8_t c = row_val[nxt][j];
                uint8_t d = row_val[nxt][j + 1];
                mvec[j] = m_tab[a * 27 + b * 9 + c * 3 + d];
            }
            pair_enc[prev * NUM_ROWS + nxt] = encodeM(mvec);

            /* store nxt in the list of rows that keep the horizontal
             * 1‑step property for this prev.                            */
            if (absdiff(mvec[0], mvec[1]) == 1 &&
                absdiff(mvec[1], mvec[2]) == 1 &&
                absdiff(mvec[2], mvec[3]) == 1) {
                horiz_next[prev][horiz_cnt[prev]++] = (uint8_t)nxt;
            }
        }
    }

    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence entry point.  n >= 1.                                 */
seq_t a(idx_t n)
{
    build_tables();

    if (n == 0) return 0;               /* not used but safe */

    /* Allocate auxiliary arrays.  All sizes are fixed and small.    */
    seq_t *dp_cur   = (seq_t*)calloc(STATE_SZ, sizeof(seq_t));
    seq_t *dp_next  = (seq_t*)calloc(STATE_SZ, sizeof(seq_t));
    bool   *visited = (bool*)calloc(STATE_SZ, sizeof(bool));

    idx_t *active_cur  = (idx_t*)calloc(STATE_SZ, sizeof(idx_t));
    idx_t *active_next = (idx_t*)calloc(STATE_SZ, sizeof(idx_t));

    size_t act_cur_sz = 0, act_next_sz = 0;

    /* ----------------------------------------------------------------
       Initialise with all pairs of rows that satisfy the horizontal rule.
       ---------------------------------------------------------------- */
    for (int r0 = 0; r0 < NUM_ROWS; ++r0) {
        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            /* mvec already computed during table build */
            const uint8_t *mvec = pair_m[r0 * NUM_ROWS + r1];
            if (absdiff(mvec[0], mvec[1]) != 1 ||
                absdiff(mvec[1], mvec[2]) != 1 ||
                absdiff(mvec[2], mvec[3]) != 1)
                continue;   /* pair fails horizontal rule */

            idx_t state = r1 * M_CODES + pair_enc[r0 * NUM_ROWS + r1];
            if (!visited[state]) {
                visited[state] = true;
                active_cur[act_cur_sz++] = state;
            }
            dp_cur[state] += (seq_t)1;
        }
    }

    /* If only two rows, sum up the initial counts. */
    if (n == 1) {
        seq_t sum = 0;
        for (size_t i = 0; i < act_cur_sz; ++i) sum += dp_cur[active_cur[i]];
        free(dp_cur); free(dp_next); free(visited);
        free(active_cur); free(active_next);
        return sum;
    }

    /* ----------------------------------------------------------------
       Extend the array by adding rows one by one.
       ---------------------------------------------------------------- */
    for (idx_t step = 1; step <= n - 1; ++step) {
        /* Clear data for the next DP level. */
        memset(dp_next, 0, STATE_SZ * sizeof(seq_t));
        memset(visited, 0, STATE_SZ * sizeof(bool));
        act_next_sz = 0;

        /* Process all states of the current level. */
        for (size_t si = 0; si < act_cur_sz; ++si) {
            idx_t state = active_cur[si];
            seq_t ways  = dp_cur[state];
            if (ways == 0) continue;

            /* Current row is the 'prev' in the pair. */
            uint8_t prev_row = (uint8_t)(state / M_CODES);
            uint16_t mprev_code = (uint16_t)(state % M_CODES);
            const uint8_t *mprev = decode_M[mprev_code];

            /* Iterate over all rows that keep the horizontal rule. */
            size_t hcnt = horiz_cnt[prev_row];
            const uint8_t *hs = horiz_next[prev_row];
            for (size_t hi = 0; hi < hcnt; ++hi) {
                uint8_t nxt = hs[hi];
                const uint8_t *mnext = pair_m[prev_row * NUM_ROWS + nxt];

                /* Vertically, all four M‑values must differ by exactly one. */
                bool ok = true;
                for (int k = 0; k < 4; ++k) {
                    int diff = (int)mnext[k] - (int)mprev[k];
                    if (diff != 1 && diff != -1) { ok = false; break; }
                }
                if (!ok) continue;

                idx_t dest = nxt * M_CODES + pair_enc[prev_row * NUM_ROWS + nxt];
                if (!visited[dest]) {
                    visited[dest] = true;
                    active_next[act_next_sz++] = dest;
                }
                dp_next[dest] += ways;
            }
        }

        /* Prepare for the next iteration.  Current data is cleared. */
        for (size_t si = 0; si < act_cur_sz; ++si)
            dp_cur[active_cur[si]] = 0;

        /* Swap pointers for next level. */
        seq_t *tmp_dp = dp_cur; dp_cur = dp_next; dp_next = tmp_dp;
        idx_t *tmp_act = active_cur; active_cur = active_next; active_next = tmp_act;
        act_cur_sz = act_next_sz;
    }

    /* Sum the counts of all final states. */
    seq_t result = 0;
    for (size_t i = 0; i < act_cur_sz; ++i) result += dp_cur[active_cur[i]];

    /* Clean up. */
    free(dp_cur); free(dp_next); free(visited);
    free(active_cur); free(active_next);

    return result;
}

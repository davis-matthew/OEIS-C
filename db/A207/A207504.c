/*
 * A207504  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 7×n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally,
 *            0 0 1 and 1 0 0 vertically. Computed by dynamic programming over
 *            column patterns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 32, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*--- 1. Pre‑compute the set of admissible column patterns -----------------*/
    static bool init_done = false;
    static size_t C;                     /* number of admissible columns   */
    static uint8_t cols[128];            /* at most 128, actually 35       */
    static uint64_t adj[128][128];       /* adjacency bit‑masks            */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static size_t DPsize = 0;            /* DPsize = C*C                    */

    if (!init_done) {
        /* find all 7‑bit patterns that avoid 001 and 100 vertically */
        C = 0;
        for (int pat = 0; pat < 128; ++pat) {
            bool ok = true;
            for (int pos = 0; pos <= 4; ++pos) {
                int b0 = (pat >> pos)     & 1;
                int b1 = (pat >> (pos+1)) & 1;
                int b2 = (pat >> (pos+2)) & 1;
                if ((b0==0 && b1==0 && b2==1) || (b0==1 && b1==0 && b2==0)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                cols[C++] = (uint8_t)pat;
            }
        }

        /* adjacency: for each pair (i,j) store all k that do not
           create forbidden 000 or 011 horizontally            */
        for (size_t i = 0; i < C; ++i) {
            for (size_t j = 0; j < C; ++j) {
                uint64_t mask = 0;
                for (size_t k = 0; k < C; ++k) {
                    bool bad = false;
                    for (int r = 0; r < 7; ++r) {
                        int b0 = (cols[i] >> r) & 1;
                        int b1 = (cols[j] >> r) & 1;
                        int b2 = (cols[k] >> r) & 1;
                        if ((b0==0 && b1==0 && b2==0) ||
                            (b0==0 && b1==1 && b2==1)) {
                            bad = true;
                            break;
                        }
                    }
                    if (!bad) mask |= (1ULL << k);
                }
                adj[i][j] = mask;
            }
        }

        /* allocate DP arrays (size C*C) */
        DPsize = C * C;
        dp_cur  = (seq_t *)malloc(DPsize * sizeof(seq_t));
        dp_next = (seq_t *)malloc(DPsize * sizeof(seq_t));
        init_done = true;
    }

    if (n == 0) return 0;

    /* Special cases for n = 1, 2 -------------------------------------------*/
    if (n == 1) return (seq_t)C;
    if (n == 2) return (seq_t)C * (seq_t)C;

    /*--- 2. DP over columns -------------------------------------------------*/
    size_t steps = (size_t)n;      /* n <= 10 000, fits in size_t */
    for (size_t i = 0; i < DPsize; ++i) dp_cur[i] = 0;
    for (size_t i = 0; i < DPsize; ++i) dp_next[i] = 0;
    /* initial state for two columns: all pairs are possible */
    for (size_t i = 0; i < DPsize; ++i) dp_cur[i] = 1;

    for (size_t step = 3; step <= steps; ++step) {
        /* zero dp_next */
        memset(dp_next, 0, DPsize * sizeof(seq_t));

        for (size_t idx = 0; idx < DPsize; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            size_t i = idx / C;
            size_t j = idx % C;
            uint64_t mask = adj[i][j];
            /* iterate over all allowed next columns k */
            while (mask) {
                uint64_t low = mask & -mask;
                size_t k = __builtin_ctzll(mask);
                mask ^= low;
                dp_next[j * C + k] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /*--- 3. Sum all states to obtain the final count ---------------------------*/
    seq_t ans = 0;
    for (size_t i = 0; i < DPsize; ++i) ans += dp_cur[i];
    return ans;
}

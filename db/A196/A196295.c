/*
 * A196295  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,0,1,4,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computed by dynamic programming over rows. State = (row pattern,
 *            3‑bit mask). The row pattern is a base‑5 representation of the
 *            three entries. The mask bit j is 1 iff the j‑th cell of the
 *            current row still needs one more neighbor equal to its target
 *            value in the next row, otherwise it must have n...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

static unsigned char target[5] = {3, 0, 1, 4, 2};

static unsigned char row_x[125][3];           /* values of each row pattern   */
static int init_done = 0;

static void init_row_values(void)
{
    for (int r = 0; r < 125; ++r) {
        int t = r;
        row_x[r][0] = t % 5; t /= 5;
        row_x[r][1] = t % 5; t /= 5;
        row_x[r][2] = t % 5;
    }
}

static bool compute_first_mask(unsigned char rid, unsigned char *mask_out)
{
    unsigned char *r = row_x[rid];
    unsigned char mask = 0;
    for (int j = 0; j < 3; ++j) {
        int v = r[j];
        int tgt = target[v];
        int known = 0;
        if (j > 0   && r[j-1] == tgt) ++known;
        if (j < 2   && r[j+1] == tgt) ++known;
        int rem = v - known;
        if (rem < 0 || rem > 1) return false;
        if (rem == 1) mask |= (1u << j);
    }
    *mask_out = mask;
    return true;
}

/* Transition tables --------------------------------------------- */
#define STATES 1000       /* 125 rows * 8 masks */
static int next_state[STATES][125];
static unsigned char next_cnt[STATES];

static void build_transitions(void)
{
    for (int rr = 0; rr < 125; ++rr) {
        unsigned char *pr = row_x[rr];
        for (int pm = 0; pm < 8; ++pm) {
            int state = rr * 8 + pm;
            unsigned char pcnt = 0;
            for (int ss = 0; ss < 125; ++ss) {
                unsigned char *pc = row_x[ss];
                bool ok = true;
                unsigned char nm = 0;

                /* Check constraint imposed by mask for transition */
                for (int j = 0; j < 3; ++j) {
                    int tgtPrev = target[pr[j]];
                    if ((pm >> j) & 1) {            /* must equal */
                        if (pc[j] != tgtPrev) { ok = false; break; }
                    } else {                         /* must differ */
                        if (pc[j] == tgtPrev) { ok = false; break; }
                    }
                }
                if (!ok) continue;

                /* Compute new mask for the next row */
                for (int j = 0; j < 3; ++j) {
                    int need = pc[j];
                    int tgt = target[need];
                    int known = 0;
                    if (j > 0   && pc[j-1] == tgt) ++known;
                    if (j < 2   && pc[j+1] == tgt) ++known;
                    if (pr[j]   == tgt)           ++known;
                    int rem = need - known;
                    if (rem < 0 || rem > 1) { ok = false; break; }
                    if (rem == 1) nm |= (1u << j);
                }
                if (!ok) continue;

                next_state[state][pcnt++] = ss * 8 + nm;
            }
            next_cnt[state] = pcnt;
        }
    }
}

/* DP ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of offset */

    if (!init_done) {
        init_row_values();
        build_transitions();
        init_done = 1;
    }

    static seq_t dp[STATES];
    static seq_t ndp[STATES];
    memset(dp, 0, sizeof(dp));

    /* first row */
    for (int rid = 0; rid < 125; ++rid) {
        unsigned char mask;
        if (!compute_first_mask((unsigned char)rid, &mask)) continue;
        dp[ rid * 8 + mask ] += (seq_t)1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (int rid = 0; rid < 125; ++rid)
            ans += dp[ rid * 8 + 0 ];
        return ans;
    }

    /* rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int s = 0; s < STATES; ++s) {
            if (dp[s] == (seq_t)0) continue;
            unsigned char cnt = next_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int ns = next_state[s][k];
                ndp[ns] += dp[s];
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* final answer: sum states with mask 0 */
    seq_t ans = 0;
    for (int rid = 0; rid < 125; ++rid)
        ans += dp[ rid * 8 + 0 ];
    return ans;
}

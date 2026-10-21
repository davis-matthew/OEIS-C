/*
 * A239043  Number of n X 4 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it, modulo 4.
 * https://oeis.org/A239043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 arrays with entries 0..3 such that for each
 *            * cell its value is not congruent to one plus the sum of the
 *            elements to its * left nor one plus the sum of the elements
 *            above, modulo 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static unsigned char value[256][4];
static unsigned char sstate[256][4];
static unsigned char trans_count[256][256];
static struct { unsigned char target; unsigned char count; } trans[256][256];
static unsigned char trans_len[256] = {0};
static bool transitions_initialized = false;

/* Precompute transition tables once. */
static void initialize_transitions(void)
{
    /* first precompute the value of each 4‑tuple (0..255) */
    for (int r = 0; r < 256; ++r) {
        unsigned int tmp = r;
        for (int j = 0; j < 4; ++j) {
            value[r][j] = tmp & 3U;
            tmp >>= 2;
        }
    }
    /* precompute the state vector for each S */
    for (int S = 0; S < 256; ++S) {
        unsigned int tmp = S;
        for (int j = 0; j < 4; ++j) {
            sstate[S][j] = tmp & 3U;
            tmp >>= 2;
        }
    }
    /* transitions for every state */
    for (int S = 0; S < 256; ++S) {
        for (int r = 0; r < 256; ++r) {
            const unsigned char *val = value[r];
            const unsigned char *sj  = sstate[S];
            int leftSum = 0;
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int L = (1 + leftSum) & 3;
                if (val[j] == L || val[j] == sj[j]) { ok = false; break; }
                leftSum = (leftSum + val[j]) & 3;
            }
            if (!ok) continue;
            unsigned char sp = ((sj[0] + val[0]) & 3) |
                               (((sj[1] + val[1]) & 3) << 2) |
                               (((sj[2] + val[2]) & 3) << 4) |
                               (((sj[3] + val[3]) & 3) << 6);
            ++trans_count[S][sp];
        }
    }
    /* compact transitions into target/count pairs */
    for (int S = 0; S < 256; ++S) {
        int len = 0;
        for (int sp = 0; sp < 256; ++sp) {
            unsigned char cnt = trans_count[S][sp];
            if (cnt) {
                trans[S][len].target = (unsigned char)sp;
                trans[S][len].count  = cnt;
                ++len;
            }
        }
        trans_len[S] = (unsigned char)len;
    }
    transitions_initialized = true;
}

seq_t a(idx_t n)
{
    if (!transitions_initialized) initialize_transitions();

    seq_t dp[256];
    seq_t newdp[256];
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    /* initial state S = (1,1,1,1) encoded as base‑4 number */
    const unsigned char start = 85;  /* ((1*4+1)*4+1)*4+1 = 85 */
    dp[start] = 1;

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < 256; ++i) newdp[i] = 0;
        for (int s = 0; s < 256; ++s) {
            if (dp[s] == 0) continue;
            seq_t val = dp[s];
            int len = trans_len[s];
            for (int t = 0; t < len; ++t) {
                int target = trans[s][t].target;
                unsigned char cnt  = trans[s][t].count;
                newdp[target] += val * (seq_t)cnt;
            }
        }
        /* copy newdp into dp */
        for (int i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < 256; ++i) ans += dp[i];
    return ans;
}

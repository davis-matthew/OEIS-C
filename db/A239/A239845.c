/*
 * A239845  Number of nX3 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or two plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A239845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2,3} with the * following
 *            constraints: * - an element cannot equal (1 + sum of elements to
 *            its left) mod 4, * - it cannot equal (1 + sum of elements above
 *            it) mod 4, * - it cannot equal (2 + sum of elements
 *            NW-diagonally) mod 4. * * This implementation uses d...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 30, u64 n <= 175, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

static unsigned char trans_cnt[1024][64];
static unsigned short trans_next[1024][64];
static unsigned char trans_limit[1024];

static void init_transitions(void)
{
    static int ready = 0;
    if (ready) return;
    ready = 1;

    int local_counts[1024];

    for (int s = 0; s < 1024; ++s)
    {
        memset(local_counts, 0, sizeof(local_counts));

        /* decode state: colSum[3] then diagSum[2] */
        int tmp = s;
        int d1 = tmp % 4; tmp /= 4;
        int d0 = tmp % 4; tmp /= 4;
        int c2 = tmp % 4; tmp /= 4;
        int c1 = tmp % 4; tmp /= 4;
        int c0 = tmp % 4;

        for (int v0 = 0; v0 < 4; ++v0)
        {
            int f0a = (1 + c0) & 3;
            int f0b = (1 + 0) & 3;    /* left sum is 0 at column 0 */
            int f0c = (2 + 0) & 3;    /* diag sum is 0 at column 0 */
            if (v0 == f0a || v0 == f0b || v0 == f0c) continue;

            int left1 = v0 & 3;
            for (int v1 = 0; v1 < 4; ++v1)
            {
                int f1a = (1 + c1) & 3;
                int f1b = (1 + left1) & 3;
                int f1c = (2 + d0) & 3;
                if (v1 == f1a || v1 == f1b || v1 == f1c) continue;

                int left2 = (v0 + v1) & 3;
                for (int v2 = 0; v2 < 4; ++v2)
                {
                    int f2a = (1 + c2) & 3;
                    int f2b = (1 + left2) & 3;
                    int f2c = (2 + d1) & 3;
                    if (v2 == f2a || v2 == f2b || v2 == f2c) continue;

                    /* valid triple found */
                    int new_c0 = (c0 + v0) & 3;
                    int new_c1 = (c1 + v1) & 3;
                    int new_c2 = (c2 + v2) & 3;
                    int new_d0 = v0 & 3;
                    int new_d1 = (d0 + v1) & 3;
                    int next = ((((((new_c0 * 4) + new_c1) * 4) + new_c2) * 4 + new_d0) * 4 + new_d1);

                    local_counts[next]++;   /* count how many assignments lead to this next state */
                }
            }
        }

        /* pack non‑zero transitions into the static tables */
        unsigned char t = 0;
        for (int ns = 0; ns < 1024; ++ns)
        {
            if (local_counts[ns])
            {
                trans_next[s][t] = (unsigned short)ns;
                trans_cnt[s][t]  = (unsigned char)local_counts[ns];
                t++;
            }
        }
        trans_limit[s] = t;
    }
}

seq_t a(idx_t n)
{
    init_transitions();

    static seq_t dp[1024];
    static seq_t nextdp[1024];

    for (int i = 0; i < 1024; ++i) dp[i] = 0;
    dp[0] = 1;                 /* initial state: all sums zero */

    for (idx_t r = 0; r < n; ++r)
    {
        for (int i = 0; i < 1024; ++i) nextdp[i] = 0;

        for (int s = 0; s < 1024; ++s)
        {
            seq_t val = dp[s];
            if (!val) continue;

            unsigned char lim = trans_limit[s];
            for (unsigned char t = 0; t < lim; ++t)
            {
                int ns = trans_next[s][t];
                unsigned char cnt = trans_cnt[s][t];
                nextdp[ns] = nextdp[ns] + val * (seq_t)cnt;
            }
        }

        /* swap dp and nextdp */
        for (int i = 0; i < 1024; ++i) dp[i] = nextdp[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 1024; ++i) total += dp[i];
    return total;
}

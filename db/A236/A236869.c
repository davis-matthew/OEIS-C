/*
 * A236869  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the upper
 *          median minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   f_of_subblock
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A236869:  number of (n+1)×3 arrays over {0,1,2}
   whose max+upper‑median−lower‑median is the same for all 2×2 subblocks. */
#include <stddef.h>
#include <string.h>
#include "../utils/f_of_subblock.h"

/* encode a triple (d0,d1,d2) of digits 0..2 into 0..26 */
static inline int encode(int d0, int d1, int d2)
{
    return d0 * 9 + d1 * 3 + d2;
}

/* decode an index 0..26 into digits 0..2 */
static inline void decode(int id, int *d0, int *d1, int *d2)
{
    *d0 = id / 9;
    *d1 = (id / 3) % 3;
    *d2 = id % 3;
}

/* transition table:  trans[a][b] = common value f of the pair
   of rows with patterns a and b, or -1 if the two adjacent 2×2 subblocks
   have different f-values. */
static signed char trans[27][27];
static int trans_ready = 0;

static void init_trans(void)
{
    if (trans_ready) return;
    for (int a = 0; a < 27; ++a) {
        int a0, a1, a2;
        decode(a, &a0, &a1, &a2);
        for (int b = 0; b < 27; ++b) {
            int b0, b1, b2;
            decode(b, &b0, &b1, &b2);
            int f1 = f_of_subblock(a0, a1, b0, b1);
            int f2 = f_of_subblock(a1, a2, b1, b2);
            trans[a][b] = (f1 == f2) ? (signed char)f1 : -1;
        }
    }
    trans_ready = 1;
}

/* main function implementing the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below the offset */
    init_trans();

    static seq_t dp_cur[5][27];
    static seq_t dp_next[5][27];
    memset(dp_cur, 0, sizeof(dp_cur));

    /* first transition: rows 0 and 1 */
    for (int p0 = 0; p0 < 27; ++p0) {
        for (int p1 = 0; p1 < 27; ++p1) {
            signed char f = trans[p0][p1];
            if (f >= 0) {
                dp_cur[(int)f][p1] += 1;
            }
        }
    }

    if (n == 1) {
        seq_t total = 0;
        for (int f = 0; f < 5; ++f)
            for (int s = 0; s < 27; ++s)
                total += dp_cur[f][s];
        return total;
    }

    /* subsequent transitions: rows 1→2, 2→3, ..., (n-1)→n */
    for (idx_t step = 2; step <= n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int f = 0; f < 5; ++f) {
            for (int cur = 0; cur < 27; ++cur) {
                seq_t val = dp_cur[f][cur];
                if (!val) continue;
                for (int nxt = 0; nxt < 27; ++nxt) {
                    if (trans[cur][nxt] == f)
                        dp_next[f][nxt] += val;
                }
            }
        }
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    seq_t total = 0;
    for (int f = 0; f < 5; ++f)
        for (int s = 0; s < 27; ++s)
            total += dp_cur[f][s];
    return total;
}

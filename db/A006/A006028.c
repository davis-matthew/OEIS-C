/*
 * A006028  Weight distribution of [ 128,99,8 ] 4th-order Reed-Muller code
 *          RM(4,7).
 * https://oeis.org/A006028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = B(2n), the number of codewords of weight 2n in RM(4,7).
 *            RM(4,7) is the dual of RM(2,7), so the MacWilliams identity
 *            gives B(j) = 2^(-29) * Sum_i A(i) * K_j(i), where K_j(i) = Sum_s
 *            (-1)^s * C(i,s) * C(128-i,j-s) (Krawtchouk polynomial). A(i) is
 *            the weight distribution of RM(2,7), from the...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128_t;

static u128_t a006028_C[129][129];
static int a006028_init = 0;

static void a006028_build(void)
{
    int i, k;
    for (i = 0; i <= 128; i++) {
        for (k = 0; k <= 128; k++) a006028_C[i][k] = 0;
        a006028_C[i][0] = 1;
        for (k = 1; k <= i; k++)
            a006028_C[i][k] = a006028_C[i - 1][k - 1] +
                              (k <= i - 1 ? a006028_C[i - 1][k] : 0);
    }
    a006028_init = 1;
}

seq_t a(idx_t n)
{
    static const int wt[9] = {0, 32, 48, 56, 64, 72, 80, 96, 128};
    static const uint64_t cnt[9] = {1ULL, 10668ULL, 5291328ULL, 112881664ULL,
                                    300503590ULL, 112881664ULL, 5291328ULL,
                                    10668ULL, 1ULL};
    u128_t total = 0;
    int j, t, s;

    if (n > 64) return (seq_t)0;
    if (!a006028_init) a006028_build();
    j = (int)(2 * n);

    for (t = 0; t < 9; t++) {
        int i = wt[t];
        u128_t kr = 0;
        for (s = 0; s <= j && s <= i; s++) {
            if (j - s > 128 - i) continue;
            {
                u128_t term = a006028_C[i][s] * a006028_C[128 - i][j - s];
                if (s & 1) kr -= term;
                else kr += term;
            }
        }
        total += (u128_t)cnt[t] * kr;
    }
    total >>= 29;
    return u128_to_seq(total);
}

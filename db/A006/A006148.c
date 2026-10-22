/*
 * A006148  Number of 4 X n binary matrices up to row and column permutations.
 * https://oeis.org/A006148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: (x^20 - x^19 + 4*x^18 + 9*x^17 + 23*x^16 + 39*x^15 +
 *            90*x^14 + 131*x^13 + 204*x^12 + 238*x^11 + 252*x^10 + 238*x^9 +
 *            204*x^8 + 131*x^7 + 90*x^6 + 39*x^5 + 23*x^4 + 9*x^3 + 4*x^2 - x
 *            + 1)/((1 - x^4)^3*(1 - x^3)^4*(1 - x^2)^3*(1 - x)^6). Computed
 *            by expanding the series: the numerator divided...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 27, u64 n <= 144, u128 n <= 1000,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void build(idx_t len) {
    static const int numer[21] = {
        1, -1, 4, 9, 23, 39, 90, 131, 204, 238, 252,
        238, 204, 131, 90, 39, 23, 9, 4, -1, 1
    };
    seq_t *s = alloc_zero(len);
    long long run = 0;
    idx_t i;
    int r, k;
    for (i = 0; i < len; i++) {
        if (i <= 20) run += numer[i];
        s[i] = (seq_t)run;
    }
    /* remaining factors: (1-x)^-5, (1-x^2)^-3, (1-x^3)^-4, (1-x^4)^-3 */
    {
        static const int step[4] = {1, 2, 3, 4};
        static const int times[4] = {5, 3, 4, 3};
        for (k = 0; k < 4; k++) {
            idx_t st = (idx_t)step[k];
            for (r = 0; r < times[k]; r++) {
                for (i = st; i < len; i++) s[i] += s[i - st];
            }
        }
    }
    if (memo) release(memo, memo_len);
    memo = s;
    memo_len = len;
}

seq_t a(idx_t n) {
    if (n >= memo_len) {
        idx_t len = n + 1;
        if (len < 2 * memo_len) len = 2 * memo_len;
        if (len < 64) len = 64;
        build(len);
    }
    return memo[n];
}

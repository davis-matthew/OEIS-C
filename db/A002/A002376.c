/*
 * A002376  Least number of positive cubes needed to sum to n.
 * https://oeis.org/A002376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + min_{k>=1, k^3<=n} a(n - k^3), a(0) = 0 (dynamic
 *            programming over cubes, memoized)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *a002376_dp = NULL;
static idx_t a002376_len = 0;

seq_t a(idx_t n) {
    if (n >= a002376_len) {
        idx_t newlen = n + 1;
        if (newlen < 2 * a002376_len) newlen = 2 * a002376_len;
        unsigned char *p = (unsigned char *)realloc(a002376_dp, (size_t)newlen);
        if (!p) return (seq_t)0;
        a002376_dp = p;
        if (a002376_len == 0) {
            a002376_dp[0] = 0;
            a002376_len = 1;
        }
        for (idx_t i = a002376_len; i < newlen; i++) {
            unsigned best = 255;
            for (idx_t k = 1; k * k * k <= i; k++) {
                unsigned c = 1u + a002376_dp[i - k * k * k];
                if (c < best) best = c;
            }
            a002376_dp[i] = (unsigned char)best;
        }
        a002376_len = newlen;
    }
    return (seq_t)a002376_dp[n];
}

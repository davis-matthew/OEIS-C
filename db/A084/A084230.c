/*
 * A084230  Cumulative minima of A006046(n)/n^theta, where
 *          theta=log(3)/log(2), is a local minimum.
 * https://oeis.org/A084230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd index m such that A006046(m)/m^theta
 *            (theta=log2(3)) is smaller than at all smaller indices
 *            (cumulative minima); search over binary prefixes using
 *            A006046(2m)=3*A006046(m),
 *            A006046(2m+1)=2*A006046(m)+A006046(m+1)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

#define A084230_TH 1.5849625007211561814537389439478L

static unsigned long long A084230_T[256];
static int A084230_cnt = 0;
static int A084230_lvl = 0;
static int A084230_k = 0;
static long double A084230_R = 1e30L;

static void A084230_dfs(unsigned long long p, int depth, long double fp, long double fp1) {
    long double g = fp / powl((long double)p, A084230_TH);
    if (depth == A084230_k) {
        if ((p & 1ULL) && g < A084230_R && A084230_cnt < 256) {
            A084230_R = g;
            A084230_T[A084230_cnt++] = p;
        }
        return;
    }
    if (g * (1.0L - A084230_TH / (long double)p - 1e-12L) >= A084230_R) return;
    A084230_dfs(2 * p, depth + 1, 3 * fp, 2 * fp + fp1);
    A084230_dfs(2 * p + 1, depth + 1, 2 * fp + fp1, 3 * fp1);
}

seq_t a(idx_t n) {
    while ((unsigned long long)A084230_cnt < (unsigned long long)n && A084230_lvl < 62 && A084230_cnt < 256) {
        A084230_lvl++;
        A084230_k = A084230_lvl;
        A084230_dfs(1ULL, 1, 1.0L, 3.0L);
    }
    if (n == 0 || (unsigned long long)n > (unsigned long long)A084230_cnt) return 0;
    return (seq_t)A084230_T[n - 1];
}

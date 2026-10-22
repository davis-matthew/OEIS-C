/*
 * A005284  Number of permutations of (1,...,n) having n-6 inversions (n>=6).
 * https://oeis.org/A005284
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-6)] Product_{j>=1} (1-x^j) / (1-x)^n = Sum_{j in Z}
 *            (-1)^j * binomial(2n-7-j(3j-1)/2, n-6-j(3j-1)/2) (Euler
 *            pentagonal theorem, valid since n-6 < n); for fixed-width builds
 *            the T(m,k) = Sum_{i=0..min(k,m-1)} T(m-1,k-i) inversion DP is
 *            used (exact mod 2^w)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t a_dp(idx_t n)
{
    idx_t K = n - 6;
    seq_t *prev = alloc_zero(K + 1);
    seq_t *curr = alloc_zero(K + 1);
    prev[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        idx_t limit = m - 1;
        seq_t sum = 0;
        for (idx_t k = 0; k <= K; k++) {
            sum += prev[k];
            if (k > limit) sum -= prev[k - limit - 1];
            curr[k] = sum;
        }
        seq_t *t = prev; prev = curr; curr = t;
    }
    seq_t r = prev[K];
    release(prev, K + 1);
    release(curr, K + 1);
    return r;
}

static seq_t a_big(idx_t n)
{
    idx_t K = n - 6;
    signed char *sg = (signed char *)calloc((size_t)K + 1, 1);
    for (idx_t j = 0;; j++) {
        idx_t g1 = j * (3 * j - 1) / 2;   /* j>=0 gives 0,1,5,12,... */
        idx_t g2 = j * (3 * j + 1) / 2;   /* -j gives 2,7,15,... */
        if (j > 0 && g1 > K) break;
        signed char s = (j & 1) ? -1 : 1;
        if (g1 <= K) sg[K - g1] = s;
        if (j > 0 && g2 <= K) sg[K - g2] = s;
        if (j == 0) continue;
    }
    seq_t pos = 0, neg = 0;
    seq_t c = 1; /* c_k = binomial(n-1+k, k) */
    for (idx_t k = 0; k <= K; k++) {
        if (k > 0) {
            c *= (seq_t)(n - 1 + k);
            c /= (seq_t)k;
        }
        if (sg[k] > 0) pos += c;
        else if (sg[k] < 0) neg += c;
    }
    free(sg);
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n < 6) return 0;
    if (seq_is_big()) return a_big(n);
    return a_dp(n);
}

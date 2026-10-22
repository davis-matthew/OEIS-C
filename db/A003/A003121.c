/*
 * A003121  Strict sense ballot numbers: n candidates, k-th candidate gets k
 *          votes.
 * https://oeis.org/A003121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(n+1,2)! * (1!*2!*...*(n-1)!) /
 *            (1!*3!*...*(2n-1)!), evaluated exactly through the prime
 *            factorization of the quotient
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    idx_t m = n * (n + 1) / 2;
    long long *cnt = (long long *)calloc((size_t)m + 2, sizeof(long long));
    uint32_t *spf = (uint32_t *)calloc((size_t)m + 1, sizeof(uint32_t));
    idx_t i, k;

    /* each factorial j! adds +-1 to the exponent of every integer 2..j */
    cnt[m] += 1;
    for (i = 1; i < n; i++) cnt[i] += 1;
    for (i = 1; i <= n; i++) cnt[2 * i - 1] -= 1;
    for (k = m - 1; k >= 1; k--) cnt[k] += cnt[k + 1];

    /* smallest prime factor sieve */
    for (k = 2; k <= m; k++) {
        if (spf[k] == 0) {
            spf[k] = (uint32_t)k;
            if (k <= m / k)
                for (idx_t j = k * k; j <= m; j += k)
                    if (spf[j] == 0) spf[j] = (uint32_t)k;
        }
    }

    /* push exponents of composites down to their factors */
    for (k = m; k >= 2; k--) {
        if (spf[k] != k && cnt[k] != 0) {
            idx_t p = spf[k];
            cnt[p] += cnt[k];
            cnt[k / p] += cnt[k];
            cnt[k] = 0;
        }
    }

    seq_t r = 1;
    for (k = 2; k <= m; k++) {
        if (spf[k] == k) {
            for (long long e = 0; e < cnt[k]; e++) r *= (seq_t)k;
        }
    }
    free(cnt);
    free(spf);
    return r;
}

/*
 * A005388  Number of degree-n permutations of order a power of 2.
 * https://oeis.org/A005388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   E.g.f.: exp(Sum_{m>=0} x^(2^m)/2^m), so a(n) = Sum_{m>=0,
 *            2^m<=n} (n-1)!/(n-2^m)! * a(n-2^m), a(0) = 1.
 *            Arbitrary-precision build: with d(k) = a(k)*N!/k!, k*d(k) =
 *            Sum_{2^m<=k} d(k-2^m), d(0) = N!, and a(N) = d(N)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (seq_is_big()) {
        seq_t *d = alloc_zero(n + 1);
        seq_t r;
        idx_t k, p;
        d[0] = 1;
        for (k = 2; k <= n; k++) d[0] *= (seq_t)k;
        for (k = 1; k <= n; k++) {
            seq_t s = 0;
            for (p = 1; p <= k; p <<= 1) s += d[k - p];
            d[k] = s / (seq_t)k;
        }
        r = d[n];
        release(d, n + 1);
        return r;
    }

    if (memo == NULL) {
        memo = alloc_zero(1);
        memo[0] = 1;
        memo_len = 1;
    }
    if (n >= memo_len) {
        idx_t k, t;
        memo = grow_zero(memo, memo_len, n + 1);
        for (k = memo_len; k <= n; k++) {
            seq_t s = 0, P = 1;
            for (t = 1; t <= k; t++) {
                if ((t & (t - 1)) == 0) s += P * memo[k - t];
                P *= (seq_t)(k - t);
            }
            memo[k] = s;
        }
        memo_len = n + 1;
    }
    return memo[n];
}

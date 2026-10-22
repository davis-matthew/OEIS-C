/*
 * A005285  Number of permutations of (1,...,n) having n-7 inversions (n>=7).
 * https://oeis.org/A005285
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j} (-1)^j * binomial(2n-8-g_j, n-1), over
 *            generalized pentagonal numbers g_j = j(3j-1)/2 <= n-7 (j in Z);
 *            i.e. a(n) = [x^(n-7)] Prod_{i=1..n} (1-x^i)/(1-x), which is the
 *            Euler pentagonal series times (1-x)^(-n) because n-7 < n
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_zero, big_path, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/big_path.h"

static seq_t fixed_path(idx_t n) {
    idx_t k = n - 7, i, j;
    seq_t *dp = alloc_zero(k + 1);
    seq_t r;
    dp[0] = 1;
    for (i = 2; i <= n; i++) {
        for (j = 1; j <= k; j++) dp[j] += dp[j - 1];
        if (i <= k)
            for (j = k; j >= i; j--) dp[j] -= dp[j - i];
    }
    r = dp[k];
    release(dp, k + 1);
    return r;
}

seq_t a(idx_t n) {
    if (n < 7) return 0;
    if (seq_is_big()) return big_path(n);
    return fixed_path(n);
}

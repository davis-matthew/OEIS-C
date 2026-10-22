/*
 * A006025  Number of labeled mating digraphs with n nodes.
 * https://oeis.org/A006025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n, k)*2^(k^2-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   A001832_pow2, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A001832_pow2.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* unsigned Stirling numbers of the first kind c(i, j), built in place */
    seq_t *c = alloc_zero(n + 1);
    c[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j >= 1; j--) {
            c[j] = c[j - 1] + c[j] * (i - 1);
        }
        c[0] = 0;
    }

    /* s(n, k) = (-1)^(n-k) c(n, k); accumulate positive and negative parts */
    seq_t pos = 0, neg = 0;
    for (idx_t k = 1; k <= n; k++) {
        seq_t term = c[k] * A001832_pow2(k * k - k);
        if (((n - k) & 1) == 0) pos += term;
        else neg += term;
    }

    release(c, n + 1);
    return pos - neg;
}

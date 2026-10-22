/*
 * A002825  Number of precomplete Post functions.
 * https://oeis.org/A002825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = -n - 2 + (-1)^(n-1) * Sum_{k=0..n-1} ((-1)^k *
 *            binomial(n, k) * Sum_{j=0..k} 2^binomial(k, j)), n > 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   a002825_p2, a002825_sadd, a002825_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a002825_p2.h"
#include "../utils/a002825_sadd.h"
#include "../utils/a002825_width.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    int w = a002825_width();

    /* binomial(n, k) row, saturating uint64 */
    uint64_t *bn = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    /* binomial(k, j) row, built incrementally, saturating uint64 */
    uint64_t *bk = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    idx_t i, j, k;

    bn[0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = i; j >= 1; j--) bn[j] = a002825_sadd(bn[j], bn[j - 1]);
    }

    seq_t pos = 0, neg = 0;
    bk[0] = 1;
    for (k = 0; k < n; k++) {
        if (k > 0) {
            for (j = k; j >= 1; j--) bk[j] = a002825_sadd(bk[j], bk[j - 1]);
        }
        seq_t inner = 0;
        for (j = 0; j <= k; j++) inner += a002825_p2(bk[j], w);
        seq_t term = (seq_t)bn[k] * inner;
        if (((n - 1) + k) % 2 == 0) pos += term;
        else neg += term;
    }

    free(bn);
    free(bk);

    seq_t r = pos - neg;
    r -= (seq_t)n;
    r -= (seq_t)2;
    return r;
}

/*
 * A006024  Number of labeled mating graphs with n nodes. Also called
 *          point-determining graphs.
 * https://oeis.org/A006024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n, k)*2^binomial(k, 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_zero, pw2, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/pw2.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t a006024_p2(idx_t e) {
    if (seq_is_fixed_width() && e >= (idx_t)(8 * sizeof(seq_t))) return 0;
    return pw2(e);
}

seq_t a(idx_t n) {
    /* c[k] = unsigned Stirling numbers of the first kind |s(i,k)| */
    seq_t *c = alloc_zero(n + 1);
    c[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t k = i; k >= 1; k--) {
            c[k] = c[k - 1] + (seq_t)(i - 1) * c[k];
        }
        c[0] = 0;
    }

    seq_t pos_sum = 0, neg_sum = 0;
    for (idx_t k = 0; k <= n; k++) {
        idx_t e = (k > 1) ? (k * (k - 1) / 2) : 0;
        seq_t term = c[k] * a006024_p2(e);
        if ((n - k) % 2 == 0)
            pos_sum += term;
        else
            neg_sum += term;
    }

    release(c, n + 1);
    return pos_sum - neg_sum;
}

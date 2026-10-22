/*
 * A000409  Singular n X n (0,1)-matrices: the number of n X n (0,1)-matrices
 *          having distinct, nonzero ordered rows, but having at least two
 *          equal columns or at least one zero column.
 * https://oeis.org/A000409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (-1)*Sum_{k=0..n-1} Stirling1(n+1, k+1)*binomial(2^k-1,
 *            n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   a409_binom_big, a409_binom_fixed, a409_fixed, a409_inv, a409_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a409_binom_big.h"
#include "../utils/a409_binom_fixed.h"
#include "../utils/a409_fixed.h"
#include "../utils/a409_inv.h"
#include "../utils/a409_mul.h"

seq_t a(idx_t n) {
    int fixed = a409_fixed();
    if (fixed && n > 60) return 0; /* cannot fit in any fixed width */

    /* unsigned Stirling numbers of the first kind, row n+1 */
    seq_t *s = (seq_t *)calloc((size_t)(n + 2), sizeof(seq_t));
    for (idx_t j = 0; j < n + 2; j++) s[j] = 0;
    s[0] = 1;
    for (idx_t i = 1; i <= n + 1; i++) {
        for (idx_t j = i; j >= 1; j--)
            s[j] = (seq_t)(s[j - 1] + a409_mul((seq_t)(i - 1), s[j]));
        s[0] = 0;
    }

    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k < n; k++) {
        if (k < 64 && ((1ULL << k) - 1) < n) continue; /* binomial is 0 */
        seq_t b = fixed ? a409_binom_fixed((1ULL << k) - 1, n)
                        : a409_binom_big(k, n);
        seq_t term = a409_mul(s[k + 1], b);
        /* sign of term is (-1)^(n-k+1) */
        if ((n - k) & 1) pos = (seq_t)(pos + term);
        else neg = (seq_t)(neg + term);
    }
    free(s);
    return (seq_t)(pos - neg);
}

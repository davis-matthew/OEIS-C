/*
 * A005512  Number of series-reduced labeled trees with n nodes.
 * https://oeis.org/A005512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n-2} (-1)^k*(n-k)^(n-k-2)*binomial(n,
 *            k)*(n-2)!/(n-k-2)!, n>=2; a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   a000272_ipow, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a000272_ipow.h"
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n) {
    if (n < 3) return 1;

    bool fixed = seq_is_fixed_width();
    seq_t *row = NULL;
    if (fixed) {
        /* Pascal row n, exact modulo 2^w (no division needed) */
        row = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        row[0] = 1;
        for (idx_t i = 1; i <= n; i++) row[i] = 0;
        for (idx_t r = 1; r <= n; r++)
            for (idx_t j = r; j >= 1; j--)
                row[j] += row[j - 1];
    }

    seq_t pos = 0, neg = 0;
    seq_t bin = 1;  /* binomial(n, k), arbitrary-precision path */
    seq_t fr = 1;   /* (n-2)!/(n-k-2)! */
    for (idx_t k = 0; k <= n - 2; k++) {
        idx_t nk = n - k;
        seq_t b = fixed ? row[k] : bin;
        seq_t term = a000272_ipow((seq_t)nk, nk - 2) * b * fr;
        if (k % 2 == 0) pos += term;
        else neg += term;

        if (!fixed) bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
        fr *= (seq_t)(n - 2 - k);
    }

    if (row) free(row);
    return pos - neg;
}

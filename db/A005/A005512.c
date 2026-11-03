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
 * Validated: standard, pass, 36 terms through n = 97 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < 3) return 1;

    bool fixed = seq_is_fixed_width();
    seq_t *row = NULL;
    if (fixed) {
        /* Pascal row n, exact modulo 2^w (no division needed) */
        row = mem_new(seq_t, (size_t)(n + 1));
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
        seq_t term = ipow_idx((seq_t)nk, nk - 2) * b * fr;
        if (k % 2 == 0) pos += term;
        else neg += term;

        if (!fixed) bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
        fr *= (seq_t)(n - 2 - k);
    }

    if (row) mem_free(row);
    return pos - neg;
}

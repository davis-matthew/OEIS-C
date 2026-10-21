/*
 * A059087  Triangle T(n,m) of number of labeled n-node T_0-hypergraphs with m
 *          distinct hyperedges (empty hyperedge excluded), m=0,1,...,2^n-1.
 * https://oeis.org/A059087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = T(r, m) = Σ_{i=0..r} s(r,i) * binomial(2^i-1, m) where r
 *            = ⌊log₂(n+1)⌋ and m = n - (2^r-1). s(r,i) are the signed
 *            Stirling numbers of the first kind
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   _index_to_row_col, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function seq_t binom(idx_t n, idx_t k). */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/_index_to_row_col.h"

#define MAX_R 65   /* enough for any n that fits in uint64_t */

static bool _stirling_inited = false;
static long long _stirling[MAX_R][MAX_R];

/* one‑time computation of signed Stirling numbers of the first kind */
static void _init_stirling(void)
{
    if (_stirling_inited) return;

    for (int n = 0; n < MAX_R; ++n)
        for (int k = 0; k < MAX_R; ++k)
            _stirling[n][k] = 0;

    _stirling[0][0] = 1;

    for (int n = 1; n < MAX_R; ++n) {
        for (int k = 1; k <= n; ++k) {
            _stirling[n][k] = _stirling[n-1][k-1] - (long long)(n-1) * _stirling[n-1][k];
        }
    }

    _stirling_inited = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* compute the row r and column m corresponding to flat index n */
    idx_t r, m;
    _index_to_row_col(n, &r, &m);

    /* ensure Stirling numbers are ready */
    _init_stirling();

    /* compute T(r,m) by the signed sum */
    seq_t result = 0;

    for (idx_t i = 0; i <= r; ++i) {
        long long s = _stirling[(int)r][(int)i];
        if (s == 0) continue;

        /* top of the binomial coefficient: 2^i - 1 */
        idx_t top = ((idx_t)1 << i) - 1;

        seq_t bin = binom(top, m);            /* binomial coefficient */
        seq_t abs_s = (seq_t)(s > 0 ? s : -s);/* absolute Stirling value */

        seq_t term = bin * abs_s;             /* product, fits if result fits */

        if (s > 0)
            result += term;
        else
            result -= term;
    }

    return result;
}

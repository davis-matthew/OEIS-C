/*
 * A178923  Rectangular array T(m,k)= StirlingS2(k-1,m-1)*m! (The Coupon
 *          Collectors Problem).
 * https://oeis.org/A178923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where (m,k) come from the diagonal enumeration of
 *            the table T(m,k) = StirlingS2(k-1,m-1) * m! The mapping from n
 *            to (m,k) is: let n0 = n-1 find the smallest integer d (>=2) such
 *            that d*(d-1)/2 > n0 // number of entries up to diagonal d let
 *            prev = (d-1)*(d-2)/2 // entries up to previ...
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt.h"   /* declares _floor_isqrt_u64(idx_t) */

static seq_t *stirling = NULL;          /* 1D array for StirlingS2 */
static seq_t *fact     = NULL;          /* factorials */
static size_t stirling_cap = 0;         /* current size of tables */

/* Ensure tables are large enough for values up to needed (inclusive) */
static void ensure_tables(size_t needed)
{
    if (needed < stirling_cap)
        return;

    size_t new_cap = needed + 1;
    seq_t *new_stirling = (seq_t *)malloc(sizeof(seq_t) * new_cap * new_cap);
    seq_t *new_fact     = (seq_t *)malloc(sizeof(seq_t) * new_cap);

    /* initialise Stirling table */
    for (size_t i = 0; i < new_cap; ++i) {
        for (size_t j = 0; j < new_cap; ++j)
            new_stirling[i * new_cap + j] = 0;
    }
    new_stirling[0] = 1;    /* S(0,0) = 1 */

    for (size_t n = 1; n < new_cap; ++n) {
        for (size_t k = 1; k <= n; ++k) {
            seq_t t1 = new_stirling[(n - 1) * new_cap + k];
            t1 = t1 * (seq_t)k;
            seq_t t2 = new_stirling[(n - 1) * new_cap + (k - 1)];
            new_stirling[n * new_cap + k] = t1 + t2;
        }
    }

    /* factorials */
    new_fact[0] = 1;
    for (size_t i = 1; i < new_cap; ++i)
        new_fact[i] = new_fact[i - 1] * (seq_t)i;

    /* free old tables */
    free(stirling);
    free(fact);

    stirling = new_stirling;
    fact     = new_fact;
    stirling_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* not defined below offset, but safe */

    /* ---------- Find diagonal d ---------- */
    idx_t n0 = n - 1;        /* 0‑based index */
    idx_t low = 2, high = n0 + 1;   /* d is at least 2 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val > n0)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;   /* minimal d with d*(d-1)/2 > n0 */

    /* previous diagonal cumulative */
    unsigned __int128 prev_val = (unsigned __int128)(d - 1) * (d - 2) / 2;
    idx_t prev = (idx_t)prev_val;
    idx_t pos   = n0 - prev;          /* 0‑based inside diagonal */
    idx_t m = pos + 1;
    idx_t k = d - m;                  /* k >= 1, m >= 1 */

    if (k < m)
        return 0;

    /* Need tables up to max(m-1, k-1) for Stirling, and up to m for factorial */
    size_t needed = k > m ? k : m;
    if (needed > 0)
        --needed;   /* because indices start at 0 in tables */

    if (needed + 1 > stirling_cap)
        ensure_tables(needed + 1);

    /* fetch StirlingS2(k-1, m-1) */
    seq_t s2 = stirling[(k - 1) * stirling_cap + (m - 1)];
    seq_t f  = fact[m];
    return s2 * f;
}

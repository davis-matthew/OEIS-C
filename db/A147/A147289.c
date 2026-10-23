/*
 * A147289  A multi-shifted and scaled Pascal triangle: t(n,m)=Binomial[n, m]
 *          + Sum[If[n >= 2*k,2*Binomial[n - 2*k, m - k], 0], {k, 1,
 *          Floor[n/2]}].
 * https://oeis.org/A147289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element (0‑based) of the flattened triangle
 *            t(n,m) defined by t(n,m) = binom(n,m) + ∑_{k=1..⌊n/2⌋} [ if n ≥
 *            2k then 2·binom(n‑2k, m‑k) else 0 ]. a(n) corresponds to the
 *            sequence obtained by concatenating all rows of t(n,m) in order
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

/* returns n*(n+1)/2 safely using 128‑bit arithmetic when available */
static inline idx_t triangular(idx_t n)
{
    return (idx_t)(((unsigned __int128)n * (n + 1)) / 2);
}

/* find the row r (0‑based) that contains the flat index idx */
static inline idx_t row_of_index(idx_t idx)
{
    /* initial estimate using the inverse of the triangular number */
    double d = sqrt(8.0 * (double)idx + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);

    /* correct possible rounding errors */
    while (idx < triangular(r)) {
        --r;
    }
    while (idx >= triangular(r + 1)) {
        ++r;
    }
    return r;
}

/* compute t(n,m) for given row n and column m  (0‑based) */
static inline seq_t compute_t(idx_t n, idx_t m)
{
    seq_t sum = binom(n, m);

    idx_t maxk = n / 2;
    for (idx_t k = 1; k <= maxk && k <= m; ++k) {
        sum += (seq_t)2 * binom(n - 2 * k, m - k);
    }
    return sum;
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t m = n - triangular(r);
    return compute_t(r, m);
}

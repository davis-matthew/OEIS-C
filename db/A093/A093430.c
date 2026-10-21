/*
 * A093430  Triangle read by rows: T(n,k) = lcm(n, n-1, ..., n-k+2,
 *          n-k+1)/lcm(1, 2, ..., k) (1 <= k <= n).
 * https://oeis.org/A093430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle * T(r,k) = lcm(r, r-1, …
 *            , r-k+1) / lcm(1, 2, … , k) * read row by row (offset 1, so
 *            a(1)=1)
 * Range:     u8 n <= 24, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* make std integer types visible */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* gcd for seq_t is provided by the build */
#include "../utils/gcd.h"

static inline idx_t find_row(idx_t n)
{
    /* find smallest r with r(r+1)/2 >= n */
    idx_t r = 1;
    while (((__int128)r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/* Return the n-th term of A093430.  idx_t >= 1 for valid inputs. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined, return zero */

    /* determine row r and column k (1-based) */
    idx_t r = find_row(n);
    idx_t prev_terms = ((idx_t)(r - 1) * r) / 2;   /* terms before row r */
    idx_t k = n - prev_terms;                      /* 1 <= k <= r */

    /* compute numerator lcm over the range [r-k+1, r] */
    seq_t numer = (seq_t)1;
    for (idx_t i = r - k + 1; i <= r; ++i)
        numer = lcm_seq(numer, (seq_t)i);

    /* compute denominator lcm over [1, k] */
    seq_t denom = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        denom = lcm_seq(denom, (seq_t)i);

    return numer / denom;
}

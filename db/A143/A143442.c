/*
 * A143442  Triangle read by rows, A127648 * A000012 * A128407, 1 <= k <= n.
 * https://oeis.org/A143442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = r * μ(k) where r is the row number of n in the triangle
 *            (i.e., the smallest integer such that r*(r+1)/2 ≥ n) and k = n -
 *            r*(r-1)/2 is the column position (1‑based). μ(k) is the Möbius
 *            function
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   fast_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fast_mu.h"
/* The following helper can be used if provided, otherwise a simple
   on‑the‑fly implementation of μ is supplied below. */
#ifndef HAVE_MOBIUS

#endif

static idx_t find_row(idx_t n)
{
    /* binary search for the smallest r with r*(r+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    /* grow hi until condition satisfied */
    while ((unsigned __int128)hi * (hi + 1u) / 2u < n)
        hi <<= 1;
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1u) / 2u;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1u;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    /* triangular number of previous row */
    idx_t prev = (idx_t)((unsigned __int128)(r - 1u) * r / 2u);
    idx_t k = n - prev;                   /* 1‑based column index */

    /* compute Möbius value */
#ifndef HAVE_MOBIUS
    int mu_val = fast_mu(k);
#else
    /* If a library implementation exists, use it. */
    extern seq_t mobius(idx_t m);
    int mu_val = (int)mobius(k);
#endif

    return (seq_t)r * (seq_t)mu_val;
}

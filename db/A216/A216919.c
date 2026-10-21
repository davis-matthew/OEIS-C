/*
 * A216919  The Gauss factorial N_n! for N >= 0, n >= 1, square array read by
 *          antidiagonals.
 * https://oeis.org/A216919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216919: Gauss factorial read by antidiagonals */
/* a(n) = product_{1 <= j <= N, gcd(j,nr)=1} j,
   where (nr,N) is obtained from n by scanning the matrix
   of values N_n! (row nr≥1, column N≥0) diagonally
   (nr + N*(nr+N+1)/2). */

#include <math.h>
#include "../utils/gcd.h"

static inline idx_t _diag_of(idx_t k)
{
    double d = sqrt((double)k * 8.0 + 1.0);
    idx_t s = (idx_t)((d - 1.0) / 2.0);

    unsigned __int128 tri = (unsigned __int128)s * (s + 1) / 2;
    if (tri < k) {
        ++s;
    } else {
        while (s > 0 &&
               (unsigned __int128)(s - 1) * s / 2 >= k)
            --s;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* Find the diagonal that contains position n */
    idx_t s = _diag_of(n);

    /* Total elements before this diagonal */
    unsigned __int128 prev128 = (unsigned __int128)s * (s - 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* Position within the diagonal (1‑based) */
    idx_t pos = n - prev;          /* 1 <= pos <= s */

    /* Row index (nr) and column (N) */
    idx_t nr = pos;                /* 1 <= nr <= s */
    idx_t N  = s - nr;             /* 0 <= N <= s-1 */

    /* Compute the Gauss factorial N_n! for this (nr,N) */
    seq_t res = (seq_t)1;
    for (idx_t j = 1; j <= N; ++j) {
        if (gcd((seq_t)j, (seq_t)nr) == (seq_t)1)
            res *= (seq_t)j;
    }
    return res;
}

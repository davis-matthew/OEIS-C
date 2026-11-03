/*
 * A162745  A Fibonacci-Pascal triangle.
 * https://oeis.org/A162745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) with r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r(r+1)/2; where T(r,k) = Σ_{j=0}^{k} C(r,j) * C(r-j, 2(k-j)) *
 *            Fib(k-j+1)
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Determine the row r and column k for the linear index n */
    idx_t r = (idx_t)((sqrt((double)8.0 * n + 1.0) - 1.0) / 2.0);

    /* Correct possible floating‑point off-by-one errors */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    seq_t sum = 0;

    for (idx_t j = 0; j <= r && j <= k; ++j)
    {
        idx_t rem = k - j;             /* k-j */
        idx_t n1  = r - j;             /* r-j */
        idx_t k1  = 2 * rem;           /* 2(k-j) */

        if (k1 > n1)
            continue;                   /* binomial is zero when k1>n1 */

        seq_t c1 = binomial(r, j);
        seq_t c2 = binomial(n1, k1);
        seq_t f  = fib(rem + 1);

        sum += c1 * c2 * f;
    }

    return sum;
}

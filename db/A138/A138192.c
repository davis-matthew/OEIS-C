/*
 * A138192  A triangular sequence based on expansion of the rational
 *          polynomial of A001788 as a Sheffer sequence:
 *          p(x,t)=Exp[x*t]*(-1/(2*t - 1)^3).
 * https://oeis.org/A138192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   Let r be the smallest integer s.t. r*(r+1)/2 >= n. * Let k = n -
 *            (r-1)*r/2 - 1 (0‑based column index within the row). * Then a(n)
 *            = ((r-1)! / k!) * binom(r - k, 2) * 2^{(r-1)-k}. * This is the
 *            coefficient of x^k in the Sheffer polynomial * P(x,r-1) from
 *            p(x,t)=Exp[x*t]/(1-2t)^3
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence starts at offset 1 */

    /* Find the row r (1‑based) that contains the n‑th term. */
    idx_t r = 1;
    while (((unsigned __int128)r * (r + 1)) / 2 < n)
        ++r;

    /* Number of terms before this row. */
    idx_t prev = ((unsigned __int128)(r - 1) * r) / 2;

    /* Zero‑based column index within the row. */
    idx_t k = n - prev - 1;            /* 0 .. r-1 */

    /* Compute (r-1)! / k! as a product from k+1 to r-1. */
    idx_t up = r - 1;                  /* = row index (0‑based) */
    seq_t ratio = 1;
    for (idx_t i = k + 1; i <= up; ++i)
        ratio *= (seq_t)i;

    /* Difference between row index and column index. */
    idx_t diff = up - k;                /* = (r-1)-k = n-k */

    /* binom(r - k, 2) == binom(diff + 2, 2). */
    seq_t bin = binom(diff + 2, 2);

    /* 2^diff */
    seq_t p2 = pow2(diff);

    /* Final value. */
    seq_t val = ratio * bin;
    val *= p2;
    return val;
}

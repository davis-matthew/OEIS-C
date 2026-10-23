/*
 * A185945  Riordan array ( (1/(1-x))^m , x*A000108(x) ), m =4.
 * https://oeis.org/A185945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (r,k) entry of the Riordan array ((1/(1-x))^4 ,
 *            x*A000108(x)). For n >= 0: r = floor((sqrt(8*n+1)-1)/2) (row
 *            index) k = n - r*(r+1)/2 (column index) If k == 0: a(n) =
 *            binomial(r+3,3) Else: a(n) = k * Σ_{i=0}^{r-k} binomial(i+3,3) *
 *            binomial(2*(r-i)-k-1, r-i-1) / (r-i) (binomial(a,b) i...
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 210, u64 n <= 703,
 *            u128 n <= 1274, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"

/* triangular number t(r) = r*(r+1)/2, computed using 128‑bit to avoid overflow */
static idx_t triangular(idx_t r)
{
    unsigned __int128 prod = (unsigned __int128)r * (r + 1);
    return (idx_t)(prod / 2);
}

/* Find the largest r such that t(r) <= n.  Uses binary search with
   128‑bit multiplication to keep the logic safe for very large n. */
static idx_t row_index(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while (triangular(hi) <= n) {
        hi <<= 1;
        if (hi == 0) {      /* reached maximum idx_t value */
            hi = (idx_t)-1;
            break;
        }
    }
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (triangular(mid) <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Sequence element a(n) */
seq_t a(idx_t n)
{
    idx_t r = row_index(n);
    idx_t offset = triangular(r);
    idx_t k = n - offset;                   /* column index (k) */

    if (k == 0) {
        /* R(r,0,4) = binom(r+3,3) */
        return binom(r + 3, 3);
    }

    seq_t sum = 0;
    /* Sum over i = 0 .. r-k */
    for (idx_t i = 0; i <= r - k; ++i) {
        seq_t term1 = binom(i + 3, 3);               /* binom(i+3,3) */
        idx_t t = r - i;                             /* t = r - i */
        seq_t term2 = binom(2 * t - k - 1, t - 1);    /* binom(2t-k-1, t-1) */
        /* term = k * term1 * term2 / t */
        seq_t prod = term1 * term2;                  /* intermediate product */
        prod = prod * (seq_t)k;                      /* multiply by k */
        prod = prod / (seq_t)t;                      /* exact division by t */
        sum += prod;
    }
    return sum;
}

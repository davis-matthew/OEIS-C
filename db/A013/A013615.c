/*
 * A013615  Triangle of coefficients in expansion of (1+8*x)^n.
 * https://oeis.org/A013615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = 8^k * C(r,k) where r = the largest integer with r*(r+1)/2
 *            ≤ n, k = n - r*(r+1)/2. This enumerates the triangle of
 *            coefficients in the expansion of (1+8*x)^r row‑by‑row, starting
 *            with a(0)=1
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow8seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow8seq.h"

/* Find the row index r for a given overall index n.
   result satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Grow high until it exceeds the cumulative count. */
    while ((high * (high + 1) / 2) <= n) {
        high <<= 1;
        if (high == 0) {          /* overflow guard – unreachable for n < 2^63 */
            high = (idx_t)-1;
            break;
        }
    }
    /* Binary search for the first high with cumulative > n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t = mid * (mid + 1) / 2;
        if (t > n)
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1;   /* low is one past the desired row */
}

/* Compute C(n,k) multiplicatively using seq_t arithmetic only. */
static seq_t binomial_seq(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result = result * (n - k + i) / i */
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);                         /* row number */
    idx_t k = n - r * (r + 1) / 2;                 /* column number in that row */

    seq_t bin = binomial_seq(r, k);                /* C(r,k)          */
    seq_t p8  = pow8seq(k);                        /* 8^k             */
    return bin * p8;                               /* 8^k * C(r,k)    */
}

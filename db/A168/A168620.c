/*
 * A168620  Table T(n,k) with the coefficient [x^k] of the polynomial
 *          5*(x+1)^n - 4*(x^n+1) in column 0<=k<=n. T(0,0)=1.
 * https://oeis.org/A168620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 for n < 4. For n >= 4, let r be the unique integer r >=
 *            2 such that s(r) = (r^2 + r)/2 + 1 <= n < ( (r+1)^2 + (r+1) )/2
 *            + 1 = s(r+1). Then let t = n - s(r). If 0 <= t <= r-2, a(n) = 5
 *            * C(r, t+1); otherwise a(n) = 1. Here C(n,k) is binomial
 *            coefficient
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, start_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the binomial helper; adjust the path as needed. */
#include "../utils/binom.h"
#include <stdint.h>
#include "../utils/start_row.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 1;

    /* Find the largest r such that start_row(r) <= n. */
    idx_t low = 2;
    idx_t high = 2;
    while (start_row(high) <= n) {
        high <<= 1;
        if (high == 0) { /* overflow guard; max r needed is well below 2^64 */
            high = (idx_t)-1;
            break;
        }
    }

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (start_row(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r   = low;
    idx_t s_r = start_row(r);
    idx_t t   = n - s_r;

    /* t must be in [0, r-2] to be inside the block; otherwise a(n)=1. */
    if (t > r - 2)
        return 1;

    idx_t k = t + 1;                     /* column index (1-based) */
    seq_t coeff = binom(r, k);            /* C(r, k) */
    coeff *= (seq_t)5;                    /* multiply by 5 */
    return coeff;
}

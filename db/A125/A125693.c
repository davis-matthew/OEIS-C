/*
 * A125693  Riordan array ((1-x)/(1-3*x), x*(1-x)/(1-3*x)).
 * https://oeis.org/A125693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 191, u64 n <= 669,
 *            u128 n <= 2494, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A125693:  Riordan array ((1-x)/(1-3*x), x*(1-x)/(1-3*x)).
 *  a(n) is the row‑major enumeration of the triangle T(n,k) defined by
 *    T(n,k) = Sum_{j=0..k+1} C(k+1,j) * C(n-j,k) * (-1)^j * 3^(n-k-j),
 *    with T(0,0)=1, T(1,0)=2, T(1,1)=1, and T(n,k)=0 if k>n.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* Determine row r and column k from linear index n. */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) r++;
    r--;                           /* row index */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;        /* column index within the row */

    /* Compute T(r,k) using the given summation. */
    seq_t sum = 0;
    for (idx_t j = 0; j <= k + 1; ++j) {
        /* Exponent of 3 must be non‑negative. */
        if (r < k + j) continue;      /* term contributes 0 */
        idx_t exp = r - k - j;

        seq_t t1 = binom(k + 1, j);
        if (t1 == 0) continue;

        idx_t n_minus_j = r - j;
        seq_t t2 = 0;
        if (k <= n_minus_j) t2 = binom(n_minus_j, k);
        if (t2 == 0) continue;

        seq_t power = pow3(exp);
        seq_t term = t1 * t2;
        term *= power;

        if (j & 1)   /* (-1)^j with j odd => subtract */
            sum -= term;
        else
            sum += term;
    }
    return sum;
}

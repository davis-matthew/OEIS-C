/*
 * A136635  Triangle, read by rows, where T(n,k) = C(n,k) * C(2^k*3^(n-k), n)
 *          for n>=k>=0.
 * https://oeis.org/A136635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = C(n,k) * C(2^k * 3^(n-k), n) (triangle read row‑by‑row)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth element of the flattened triangle (offset 0). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp)       */
#include "../utils/pow3.h"   /* seq_t pow3(idx_t exp)       */

/* Return binomial coefficient C(N,k) where N is a big integer (seq_t) and
   k is a small integer (idx_t).  Assumes k ≤ N (in value).  The result
   is computed multiplicatively, always staying exact. */
static seq_t binom_big(seq_t N, idx_t k)
{
    seq_t result = 1;
    if (k == 0) return result;

    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (N - i + 1) and divide by i. */
        seq_t mult = N - (seq_t)(i - 1);
        result = result * mult / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find row r such that n lies in row r.  Triangular numbers are
       T(r) = r(r+1)/2. */
    idx_t r = 0;
    idx_t tri = 0;  /* current triangular number r(r+1)/2 */
    while (tri + r + 1 <= n) {
        tri += r + 1;
        ++r;
    }
    idx_t k = n - tri;   /* column index within the row */

    /* Compute T(r,k) = C(r,k) * C(2^k * 3^(r-k), r) */
    seq_t binom_row = binom_big((seq_t)r, k);       /* C(r,k) */
    seq_t pow2k   = pow2(k);                       /* 2^k   */
    seq_t pow3rk  = pow3(r - k);                   /* 3^(r-k) */
    seq_t N = pow2k * pow3rk;                     /* 2^k * 3^(r-k) */
    seq_t binom_big_part = binom_big(N, r);        /* C(N, r) */
    return binom_row * binom_big_part;
}

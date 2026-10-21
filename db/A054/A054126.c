/*
 * A054126  Odd-index Fibonacci row-sum array: T(n,k)=U(2n+1,n+1+k), 0<=k<=n,
 *          n >= 0, U the array in A054125.
 * https://oeis.org/A054126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, eigen
 * Formula:   a(n) = 2 if k=n; otherwise
 *            a(n)=2^(r-1-k)+2^(r+k)-∑_{m=0}^{2k-1}C(r+k,m), where r = largest
 *            integer with r(r+1)/2 ≤ n, k = n − r(r+1)/2
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t base = r * (r + 1) / 2;  /* starting index of row r */
    idx_t k = n - base;           /* column within the row */

    if (k == r)                   /* diagonal element */
        return (seq_t)2;

    /* Term 1: 2^(r-1-k) */
    seq_t term1 = pow2((idx_t)(r - 1 - k));

    /* Term 2: 2^(r+k) */
    seq_t term2 = pow2((idx_t)(r + k));

    /* Sum of binomial coefficients up to 2k-1. */
    seq_t sum = 0;
    if (k > 0) {
        idx_t limit = 2 * k - 1;
        for (idx_t m = 0; m <= limit; ++m)
            sum += binomial(r + k, m);
    }

    return term1 + term2 - sum;
}

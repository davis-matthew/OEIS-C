/*
 * A212362  Triangle by rows, binomial transform of the beheaded Pascal's
 *          triangle A074909.
 * https://oeis.org/A212362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = 2^(n-k)*C(n+1,k) +
 *            (2^(n-k)-1)*C(n,k-1), with T(n,0)=2^n. The index n is converted
 *            to triangle coordinates (r,k) by row‑major traversal: start(r) =
 *            r*(r+1)/2
 * Range:     u8 n <= 22, u16 n <= 68, u32 n <= 257, u64 n <= 914,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find row r and column k from the index n. */
    idx_t r = 0;
    /* start(r) = r*(r+1)/2; we find the smallest r such that start(r) > n, then r--. */
    while ((r * (r + 1u)) / 2 <= n) {
        r++;
    }
    r--;                                  /* now start(r) <= n < start(r+1) */
    idx_t k = n - (r * (r + 1u)) / 2;     /* column within the row */

    /* Compute T(r,k). */
    if (k == 0u) {
        return pow2(r);                  /* T(r,0) = 2^r */
    } else {
        seq_t c1 = binomial(r + 1u, k);                 /* C(r+1, k)   */
        seq_t pow_part = pow2(r - k);                    /* 2^(r-k)     */
        seq_t term1 = c1 * pow_part;                    /* first part  */
        seq_t c2 = binomial(r, k - 1u);                 /* C(r, k-1)   */
        seq_t term2 = (pow_part - (seq_t)1) * c2;        /* second part */
        return term1 + term2;                           /* T(r,k)      */
    }
}

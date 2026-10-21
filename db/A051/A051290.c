/*
 * A051290  Triangular array T read by rows: T(n,k)=P(2n+3,n,2k+3), where
 *          P(n,k,c)=number of vectors (x(1),x(2,),...,x(n)) of k 1's and n-k
 *          0's such that x(i)=x(n+1-i) for exactly c values of i.
 * https://oeis.org/A051290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle T(n,k) by rows, where T(n,k) =
 *            P(2n+3,n,2k+3) and P(N,K,C) counts 0/1 vectors of length N with
 *            K ones that satisfy x(i)=x(N+1-i) in exactly C positions. This
 *            can be computed combinatorially as let M = n+1 (outer pair
 *            count), a = ⌊k/2⌋, b = (k+1)-a, c = n-k. Then T(n,k) = C...
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h" /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp); */

/* Helper: compute T(n,k) */
static seq_t T_val(idx_t n, idx_t k)
{
    idx_t outer_pairs = n + 1;          /* number of outer pairs in length 2n+3 */
    idx_t a = k / 2;                    /* pairs that are equal and both 1 */
    idx_t b = (k + 1) - a;              /* pairs that are equal and both 0 */
    /* binomial(outer, a) * binomial(outer-a, b) * 2^(n-k) */
    seq_t result = binomial(outer_pairs, a);
    result *= binomial(outer_pairs - a, b);
    result *= pow2(n - k);
    return result;
}

/* Main sequence entry */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t k = n - r * (r + 1) / 2;  /* position within the row */
    return T_val(r, k);
}

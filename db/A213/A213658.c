/*
 * A213658  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with k vertices of the graph G(n) consisting of
 *          an edge ab and n triangles, each having one vertex identified with
 *          the vertex b.
 * https://oeis.org/A213658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry when the triangle * T(r,k) = 2^(2*r+1-k)
 *            * C(r, k-r-1) + C(2*r+1, k-1) * (1 ≤ k ≤ 2r+2) is read left to
 *            right, top to bottom. * The function finds the row r and column
 *            k corresponding to the * global index n and evaluates the
 *            formula using the provided * binomial and pow2 h...
 * Range:     u8 n <= 22, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

/* Compute the nth term of A213658 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; n must be >=1 for valid queries */
        return 0;

    /* Find the row r such that r(r+3) >= n > (r-1)(r+2) */
    idx_t r = 1;
    while ((r * (r + 3)) < n)
        ++r;

    /* Number of terms before row r */
    idx_t before = (r - 1) * (r + 2);

    /* Column index within the row (1‑based) */
    idx_t k = n - before;                     /* 1 … 2r+2 */

    seq_t val = 0;

    /* First part: 2^(2r+1-k) * C(r, k-r-1) */
    if (k >= r + 1 && k <= 2 * r + 1) {
        idx_t exp = 2 * r + 1 - k;             /* exponent ≥ 0 */
        idx_t idx = k - r - 1;                /* 0 … r-1 */
        val += pow2(exp) * binomial(r, idx);
    }

    /* Second part: C(2r+1, k-1) (always defined) */
    val += binomial(2 * r + 1, k - 1);

    return val;
}

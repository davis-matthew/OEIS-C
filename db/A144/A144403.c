/*
 * A144403  Triangle T(n, k) = binomial(n, k)^2 - binomial(n, k) - 1, read by
 *          rows.
 * https://oeis.org/A144403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) reads the triangle T(r,k) = C(r,k)^2 - C(r,k) - 1 row‑wise.
 *            For a given sequence index n, r is the unique row such that
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2, and k = n - r(r+1)/2. The term is
 *            a(n) = C(r,k)^2 - C(r,k) - 1. All values are computed exactly in
 *            seq_t
 * Range:     i8 n <= 22, i16 n <= 58, i32 n <= 179, i64 n <= 644,
 *            i128 n <= 1325, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial(r, k) exactly using integer arithmetic and gcd to avoid overflow. */
static seq_t choose(idx_t r, idx_t k) {
    if (k > r - k)
        k = r - k;           /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = r - k + i;   /* numerator part for current step */
        seq_t denom = i;         /* denominator part for current step */
        seq_t g = gcd(num, denom);
        num /= g;
        denom /= g;
        seq_t g2 = gcd(result, denom);
        result /= g2;
        denom /= g2;             /* denom should now be 1 */
        result *= num;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow in intermediate products. */
    while ( ((__int128)(r + 1) * (r + 2)) / 2 <= n )
        ++r;

    /* k is the offset within the row. */
    idx_t k = n - ( (__int128)r * (r + 1) ) / 2;

    seq_t bin = choose(r, k);
    seq_t res = bin * bin - bin - 1;
    return res;
}

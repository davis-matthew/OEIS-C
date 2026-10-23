/*
 * A126181  Triangle read by rows, T(n,k) = C(n,k)*Catalan(n-k+1), n >= 0, 0
 *          <= k <= n.
 * https://oeis.org/A126181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = ⌊(√(8n+1) – 1)/2⌋, * k = n – r(r+1)/2,
 *            * T(r,k) = binomial(r,k) * catalan(r−k+1) * (triangle read by
 *            rows)
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, catalan, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Use integer arithmetic with integer square root. */
    seq_t disc = (seq_t)8 * n + 1;          /* discriminant 8n+1  */
    seq_t s = floor_sqrt(disc);           /* floor sqrt(8n+1) */
    idx_t r = ((idx_t)s - 1) / 2;          /* floor((sqrt-1)/2) */
    /* Correct r in case of rounding issues */
    if ((idx_t)r * (r + 1) / 2 > n) {
        --r;
    } else {
        while (((idx_t)(r + 1) * (r + 2) / 2) <= n) {
            ++r;
        }
    }
    /* Position within row */
    idx_t start = (idx_t)r * (r + 1) / 2;
    idx_t k = n - start;
    /* Compute T(r,k) */
    seq_t bin = binomial(r, k);
    seq_t cat = catalan(r - k + 1);
    return bin * cat;
}

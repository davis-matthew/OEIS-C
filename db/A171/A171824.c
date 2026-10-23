/*
 * A171824  Triangle T(n,k)= binomial(n + k,n) + binomial(2*n-k,n) read by
 *          rows.
 * https://oeis.org/A171824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r + k, r) + binomial(2*r - k, r) where r =
 *            floor((sqrt(8*n + 1) - 1)/2) and k = n - r*(r+1)/2 (i.e., a(n)
 *            is the (r,k) entry of the triangle read by rows)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 170, u64 n <= 594,
 *            u128 n <= 1325, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Return the starting index of row r in the flattened sequence:
   start(r) = r*(r+1)/2  (using seq_t arithmetic to avoid overflow). */
static inline idx_t row_start(idx_t r)
{
    return (idx_t)(( (seq_t)r * ((seq_t)r + 1) ) / 2);
}

/* Compute binomial coefficient C(n,k) for idx_t arguments and seq_t result.
   The algorithm uses gcd cancellations to keep intermediate values
   small and produces the exact result as seq_t. */
static inline seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);    /* numerator */
        seq_t den = (seq_t)i;              /* denominator */

        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;

        res = res * num;
        /* After cancellations den should be 1; if not, divide it out. */
        if (den != (seq_t)1) {
            res /= den;
        }
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find row r such that row_start(r) <= n < row_start(r+1). */
    idx_t low = 0;
    idx_t high = n + 1;            /* guarantees high > r */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (row_start(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t k = n - row_start(r);

    /* Compute T(r,k) = C(r + k, r) + C(2*r - k, r). */
    seq_t term1 = binomial_calc(r + k, r);
    seq_t term2 = binomial_calc(2 * r - k, r);
    return term1 + term2;
}

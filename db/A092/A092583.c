/*
 * A092583  Triangle read by rows: T(n,k) is the number of permutations p of
 *          [n] in which the length of the longest initial segment avoiding
 *          the 123-pattern is equal to k.
 * https://oeis.org/A092583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer such that r*(r+1)/2 >=
 *            n and k = n - r*(r-1)/2. T(r,k) = Catalan(r) for k == r 0 for k
 *            < 2 prod_{i=k+2}^{r} i * binomial(2k, k-2) otherwise
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 92, u64 n <= 232, u128 n <= 596,
 *            big any n
 * Helpers:   binom_multiplicative, catalan_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_multiplicative.h"
#include "../utils/catalan_value.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside of the defined domain */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Number of elements before row r */
    idx_t prev = ((r - 1) * r) / 2;
    /* Column index (1‑based) */
    idx_t k = n - prev;

    if (k == r) {
        return catalan_value(r);
    }
    if (k < 2) {
        return 0;
    }

    /* binomial(2k, k-2) */
    seq_t bin_val = binom_multiplicative(2 * k, k - 2);

    /* product_{i=k+2}^{r} i */
    seq_t prod = 1;
    for (idx_t i = k + 2; i <= r; ++i) {
        prod *= (seq_t)i;
    }

    return prod * bin_val;
}

/*
 * A178780  Expansion of the polynomial
 *          x^36*(x^12-1)*(x^9-1)*(x^8-1)*(x^6-1)*(x^5-1)*(x^2-1) in
 *          increasing powers of x.
 * https://oeis.org/A178780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full, easy
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in
 *   x^36 * (x^12-1) * (x^9-1) * (x^8-1) * (x^6-1) * (x^5-1) * (x^2-1)
 * (i.e. coefficient of x^{n-36} in the product of (x^k-1) for k = 12,9,8,6,5,2).
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* constant exponents of the factors (excluding the initial shift of 36) */
    static const idx_t ks[] = { 12, 9, 8, 6, 5, 2 };
    const size_t kcnt = sizeof(ks) / sizeof(ks[0]);

    /* sum of all ks = 42, degree of product without the initial shift */
    const idx_t maxSum = 42;
    /* final maximum exponent after adding the initial x^36 shift */
    const idx_t maxExp = 36 + maxSum;

    /* computed on first use */
    static seq_t coeff[43];        /* indexes 0 .. 42 */
    static int ready = 0;

    if (!ready) {
        /* initialize coefficients to 0; static global array is zero‑initialized anyway */
        coeff[0] = 1;              /* empty subset contributes +1 at exponent 0 */

        /* for each factor (x^k - 1), update states in reverse to avoid re‑use */
        for (size_t fi = 0; fi < kcnt; ++fi) {
            idx_t k = ks[fi];
            for (idx_t s = maxSum; s >= k; --s) {
                /* coeff[s] gets contribution from coefficient at s-k with sign -1 */
                coeff[s] = coeff[s] - coeff[s - k];
            }
        }
        ready = 1;
    }

    /* Outside the polynomial support return 0 */
    if (n < 36 || n > maxExp)
        return 0;

    return coeff[n - 36];
}

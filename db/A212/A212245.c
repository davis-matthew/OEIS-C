/*
 * A212245  Number of (w,x,y,z) with all terms in {1,...,n} and w = x*y*z -
 *          2*n.
 * https://oeis.org/A212245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (x,y,z) ∈ [1,n]^3 | 2n+1 ≤ x*y*z ≤ 3n }
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are predefined integer types in the build environment. */

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the count is 0; this is handled naturally by the loops. */
    seq_t result = 0;

    /* Denominator bounds for the product X*Y*Z. */
    idx_t L = 2 * n + 1;   /* lower bound for X*Y*Z */
    idx_t U = 3 * n;       /* upper bound for X*Y*Z */

    /* Enumerate X and Y; for each pair find admissible Z. */
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            idx_t prod = x * y;
            if (prod > U)        /* further y will only increase prod */
                break;

            /* Compute admissible Z range: ceil(L/prod) … floor(U/prod) */
            idx_t z_min = (L + prod - 1) / prod;
            if (z_min < 1) z_min = 1;
            idx_t z_max = U / prod;
            if (z_max > n) z_max = n;

            if (z_max >= z_min)
                result += (seq_t)(z_max - z_min + 1);
        }
    }

    return result;
}

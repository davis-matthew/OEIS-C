/*
 * A083486  Triangle read by rows in which the n-th row contains the smallest
 *          set of n increasing numbers beginning with n with a product which
 *          is a square.
 * https://oeis.org/A083486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   factor_u64, floor_sqrt, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A083486 – Triangle of smallest sets of n increasing numbers starting at n
 *  whose product is a perfect square.
 *  For an index n (offset 1), find the row r such that
 *  1 + 2 + … + r-1 < n ≤ 1 + 2 + … + r.
 *  The first r-1 entries of that row are r, r+1, …, 2r-2.
 *  The last entry m is the smallest integer > 2r-2 such that
 *  (∏_{k=r}^{2r-2} k) * m is a square.
 *  This is obtained by multiplying the square‑free kernel of the product
 *  by the smallest square exceeding the remaining bound.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   // factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*)
#include "../utils/mul_small.h"    // mul_small(seq_t, unsigned int)
#include "../utils/floor_sqrt.h"   // floor_sqrt(seq_t)

seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n ≤ T(r) */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        r++;

    /* Position within the row (1‑based) */
    idx_t prev_rows = r * (r - 1) / 2;
    idx_t pos = n - prev_rows;          /* 1 ≤ pos ≤ r */

    /* Row 1 is a single element 1 */
    if (r == 1) {
        return (seq_t)1;
    }

    /* If the position is before the last element, return the consecutive number */
    if (pos <= r - 1) {
        return (seq_t)(r + pos - 1);
    }

    /* Compute the square‑free kernel of the product of the first r‑1 numbers */
    const size_t MAX_PRIMES = 64;
    uint64_t primes[MAX_PRIMES];
    uint64_t exps[MAX_PRIMES];
    size_t pcnt;

    /* Store primes that currently have an odd exponent */
    uint64_t odd[64];
    size_t oddcnt = 0;

    for (idx_t i = r; i <= 2 * r - 2; ++i) {
        factor_u64((uint64_t)i, primes, exps, &pcnt);
        for (size_t k = 0; k < pcnt; ++k) {
            if (exps[k] & 1ULL) {                 /* odd exponent */
                uint64_t p = primes[k];
                /* toggle presence of p in odd[] */
                bool found = false;
                for (size_t j = 0; j < oddcnt; ++j) {
                    if (odd[j] == p) {
                        odd[j] = odd[oddcnt - 1];
                        --oddcnt;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    odd[oddcnt++] = p;
                }
            }
        }
    }

    /* Build the kernel (product of odd primes) as seq_t */
    seq_t kernel = (seq_t)1;
    for (size_t j = 0; j < oddcnt; ++j) {
        kernel = mul_small(kernel, (unsigned int)odd[j]);
    }

    /* Compute the minimal last element m > bound = 2r-2 */
    seq_t bound = (seq_t)(2 * r - 2);
    seq_t bound_div = bound / kernel;                 /* integer division */
    seq_t t0 = floor_sqrt(bound_div);
    seq_t t = t0 + 1;                                 /* smallest t with t^2 > bound_div */

    /* Ensure t is at least 1 */
    if (t == 0) { t = (seq_t)1; }

    seq_t t2 = mul_small(t, (unsigned int)t);
    seq_t m  = mul_small(kernel, (unsigned int)t2);

    return m;
}

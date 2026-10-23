/*
 * A124320  Triangle read by rows: T(n,k) = k!*binomial(n+k-1,k) (n >= 0, 0 <=
 *          k <= n), rising factorial power, Pochhammer symbol.
 * https://oeis.org/A124320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for n ≥ 0, flatten the triangle T(r,k)=k!*C(r+k-1,k) by rows:
 *            a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋, k = n − r*(r+1)/2. The
 *            sequence starts 1,1,1,1,2,6,
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 134,
 *            u128 n <= 349, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/factorial.h"  /* seq_t factorial(idx_t n); */
#include <math.h>                /* sqrt() for index calculation */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Special case: first element of the triangle */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r that contains index n.  The cumulative
       number of elements up to the start of row r is r*(r+1)/2.
       Solve r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
    idx_t r = (idx_t)((sqrt((double)(1.0 + 8.0 * (double)n)) - 1.0) / 2.0);

    /* Correct possible rounding errors from the floating‑point
       computation.  All operations below are on 64‑bit unsigned integers. */
    idx_t start = r * (r + 1) / 2;
    if (start > n) {
        do {
            r--;
            start = r * (r + 1) / 2;
        } while (start > n);
    } else {
        while ((r + 1) * (r + 2) / 2 <= n)
            r++;
    }

    /* Column index k within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k) = k! * binomial(r + k - 1, k) */
    idx_t n_binom = r + k - 1;      /* valid for all n > 0 */
    seq_t bin = binomial(n_binom, k);
    seq_t fact = factorial(k);

    return fact * bin;
}

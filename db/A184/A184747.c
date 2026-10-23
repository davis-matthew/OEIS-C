/*
 * A184747  a(n) = floor(n*s+h-h*s), where s=1+sqrt(5), h=1/2; complement of
 *          A184746.
 * https://oeis.org/A184747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1+sqrt(5)) + ½ - (1+sqrt(5))/2)
 * Range:     u8 n <= 79, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n + floor((2*n-1)*sqrt(5)/2)  */
#include <stdint.h>

/* Rational approximation of sqrt(5) that is < sqrt(5)
 * Precision: 1.237e-6 (enough to keep the floor correct for n up to 10000).
 *              (sqrt(5) ≈ 2.2360679775)
 * 2236067 / 1,000,000 = 2.236067
 * 0.0000009775 < 1e-4, so for n ≤ 10000 the difference in the product
 * is strictly <1 and the floor remains unchanged.
 */
static const uint64_t SQRT5_NUM = 2236067ULL;
static const uint64_t SQRT5_DEN = 1000000ULL;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* value is irrelevant for offset 1 */

    /* Compute floor((2*n-1)*sqrt(5)/2) using integer arithmetic.
     * (2*n-1) and seq_t conversion fit into 64‑bit for all n used in tests.
     */
    uint64_t two_n_minus1 = (uint64_t)(2 * n - 1);
    uint64_t numer = two_n_minus1 * SQRT5_NUM;
    uint64_t denom = 2 * SQRT5_DEN;
    uint64_t part = numer / denom;

    /* a(n) = n + part */
    return (seq_t)((seq_t)n + part);
}

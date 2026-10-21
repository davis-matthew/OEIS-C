/*
 * A069950  Expansion of
 *          (1+x^2)*(1+x^5)*(1+x^8)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)*(1-x^5)*(1-x^6)*(1-x^7)*(1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A069950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (1+x^2)(1+x^5)(1+x^8) /
 *            ∏_{j=1}^{10} (1-x^j)
 * Range:     u8 n <= 13, u16 n <= 41, u32 n <= 187, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the numerator polynomial
   (1+x^2)(1+x^5)(1+x^8) and convolves it with the
   partition numbers into parts at most 10.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for partition numbers p[0..n] */
    size_t sz = (size_t)(n + 1);
    seq_t *p = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!p) {
        /* In the unlikely event that allocation fails, return 0 */
        return 0;
    }

    /* initialize */
    for (size_t i = 0; i < sz; ++i)
        p[i] = 0;
    p[0] = 1;

    /* DP: p[k] = number of partitions of k into parts 1..10 */
    for (idx_t part = 1; part <= 10; ++part) {
        for (idx_t i = part; i <= n; ++i)
            p[i] = p[i] + p[i - part];
    }

    /* numerator coefficients: 1, x^2, x^5, x^7, x^8, x^10, x^13, x^15 */
    static const idx_t sums[8] = {0, 2, 5, 7, 8, 10, 13, 15};

    seq_t res = 0;
    for (idx_t k = 0; k < 8; ++k) {
        idx_t s = sums[k];
        if (n >= s)
            res = res + p[n - s];
    }

    free(p);
    return res;
}

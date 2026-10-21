/*
 * A056096  Maximum value in the distribution by first value of Prufer code of
 *          noncrossing spanning trees on a circle of n+2 points; perhaps the
 *          number whose Prufer code starts with 2.
 * https://oeis.org/A056096
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (1/(n-3)) * Σ_{i=1}^{n-3} i*(i+1)^2 * C(3*(n-3)-i-1,
 *            (n-3)-i) where C(a,b) is the binomial coefficient
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
 *  - For n = 3 the sequence value is 1.
 *  - For n ≥ 4 let m = n-3.  The term a(n) is computed as
 *        a(n) = (sum_{i=1}^{m} i*(i+1)^2 * binom(3m - i - 1, m - i)) / m
 *    The binomial values are generated iteratively to avoid repeated
 *    expensive calls to the binomial routine.  The recurrence
 *        binom(N-1, K-1) = binom(N, K) * K / N
 *    is used with N = 3m - i - 1 and K = m - i.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 3)        /* first term in the offset 3 sequence */
        return (seq_t)1;

    /* m = n-3 must be ≥ 1 for n ≥ 4 */
    idx_t m = n - 3;
    seq_t sum = 0;

    /* Initial binomial: binom(2m-1, 0) = 1 */
    seq_t binom = 1;
    idx_t N = 2 * m - 1;   /* current N in binom(N, K) */
    idx_t K = 0;           /* current K */

    for (idx_t i = m; i >= 1; --i) {
        /* (i+1)^2 * i */
        seq_t factor = ((seq_t)i * (seq_t)(i + 1) * (seq_t)(i + 1));
        sum += binom * factor;

        if (i > 1) {
            /* Move to binom(N+1, K+1) using the recurrence:
             * binom(N+1, K+1) = binom(N, K) * (N+1) / (K+1)  */
            N += 1;
            K += 1;
            binom = (binom * (seq_t)N) / (seq_t)K;
        }
    }

    return sum / (seq_t)m;
}

/*
 * A099044  a(n) = (2*0^n + 3^n*binomial(2*n,n))/3.
 * https://oeis.org/A099044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*0^n + 3^n*binomial(2*n,n))/3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, use 0^0 = 1; for n >= 1, 0^n = 0 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) {
        /* a(0) = (2*1 + 3^0*1)/3 = 1 */
        return (seq_t)1;
    }

    /* Compute central binomial coefficient C(2n, n) */
    seq_t bin = binomial(2 * n, n);

    /* Compute 3^(n-1) */
    seq_t pow = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow *= (seq_t)3;
    }

    /* a(n) = 3^(n-1) * C(2n, n)  */
    return bin * pow;
}

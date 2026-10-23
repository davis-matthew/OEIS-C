/*
 * A101091  Fourth partial sums of fourth powers (A000583).
 * https://oeis.org/A101091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(1 + n)*(2 + n)^2*(3 + n)*(4 + n)*(-1 + 3*n*(4 +
 *            n))/5040
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   cancel_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cancel_factor.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; behaviour below 1 is irrelevant */

    uint64_t u = n;
    uint64_t f1 = u;                 /* n           */
    uint64_t f2 = u + 1;             /* n + 1       */
    uint64_t f6 = (u + 2) * (u + 2); /* (n + 2)^2   */
    uint64_t f3 = u + 3;             /* n + 3       */
    uint64_t f4 = u + 4;             /* n + 4       */
    uint64_t f7 = 3 * u * f4 - 1;    /* 3*n*(n+4)-1 */

    int den[4] = {4, 2, 1, 1};        /* 2^4 * 3^2 * 5 * 7 */
    const uint64_t primes[4] = {2, 3, 5, 7};

    seq_t prod = 1;

    cancel_factor(&f1, den, primes);
    prod *= (seq_t)f1;

    cancel_factor(&f2, den, primes);
    prod *= (seq_t)f2;

    cancel_factor(&f6, den, primes);
    prod *= (seq_t)f6;

    cancel_factor(&f3, den, primes);
    prod *= (seq_t)f3;

    cancel_factor(&f4, den, primes);
    prod *= (seq_t)f4;

    cancel_factor(&f7, den, primes);
    prod *= (seq_t)f7;

    /* At this point den should be all zeros; the product equals the desired value. */
    return prod;
}

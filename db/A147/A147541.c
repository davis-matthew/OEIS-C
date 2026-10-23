/*
 * A147541  Result of using the primes as coefficients in an infinite
 *          polynomial series in x and then expressing this series as
 *          (1+x)(1+a(1)*x)(1+a(2)*x^2) ... .
 * https://oeis.org/A147541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Recursively factor the series 1+prime(1)x+prime(2)x^2+... as
 *            (1+x)(1+a(1)x)(1+a(2)x^2).... a(n) is the coefficient of x^n in
 *            the quotient after removing all previous factors
 * Range:     i8 n <= 21, i16 n <= 37, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* offset 1: not used */
    size_t N = (size_t)n;                           /* cast to size_t for loops */

    /* Allocate array for coefficients up to degree N */
    seq_t *coeff = (seq_t*)malloc(sizeof(seq_t) * (N + 1));
    if (!coeff) return 0;                           /* allocation failed */

    coeff[0] = (seq_t)1;                            /* constant term */

    /* Initialise with the prime series */
    for (size_t i = 1; i <= N; ++i)
        coeff[i] = (seq_t)get_prime((idx_t)i);

    /* Divide by (1 + x) */
    for (size_t i = 1; i <= N; ++i)
        coeff[i] = coeff[i] - coeff[i - 1];

    /* Successively remove factors (1 + a(k) x^k) for k = 1 .. N-1 */
    for (size_t k = 1; k < N; ++k) {
        seq_t ak = coeff[k];                        /* current a(k) */
        for (size_t i = k; i <= N; ++i)
            coeff[i] = coeff[i] - ak * coeff[i - k];
    }

    seq_t result = coeff[N];
    free(coeff);
    return result;
}

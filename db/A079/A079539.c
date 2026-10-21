/*
 * A079539  a(n) = sigma[k](n) - phi(n)^k - d(n)^k for k=3.
 * https://oeis.org/A079539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma_k(n,3) - phi(n)^3 - d(n)^3
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 1229, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, phi, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/sigma_k.h"
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

/* Compute A079539(a(n)) */
seq_t a(idx_t n)
{
    /* Sum of cubes of divisors */
    seq_t sigma3 = sigma_k(n, 3);

    /* Euler's totient */
    seq_t ph = phi(n);

    /* Cube of the totient */
    seq_t ph3 = ph * ph * ph;

    /* Number of divisors */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);

    /* Cube of the divisor count */
    seq_t d3 = d * d * d;

    /* Final result */
    return sigma3 - ph3 - d3;
}

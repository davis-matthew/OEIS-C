/*
 * A089496  a(n) = mu(prime(n)+1) + mu(prime(n)-1), where mu is the Moebius
 *          function.
 * https://oeis.org/A089496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(prime(n)+1) + mu(prime(n)-1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (the function is only defined for n >= 1). */
    if (n == 0) return 0; /* Undefined behaviour below offset; return 0. */

    /* Retrieve the nth prime (1‑based indexing). */
    uint32_t p = get_prime(n);

    /* Compute μ(p+1) and μ(p-1). */
    seq_t mu_plus  = mobius((idx_t)p + 1U);
    seq_t mu_minus = mobius((idx_t)p - 1U);

    /* Return the sum. */
    return mu_plus + mu_minus;
}

/*
 * A063084  a(n) = pi(n-1)*n - pi(n)*(n-1), where pi() = A000720().
 * https://oeis.org/A063084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = pi(n-1)*n - pi(n)*(n-1)
 * Range:     i8 n <= 172, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 101 terms through n = 991 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Ensure we do not underflow when n == 0 */
    if (n == 0) return (seq_t)0;

    seq_t pi_n        = prime_pi(n);
    seq_t pi_n_minus1 = prime_pi(n - 1);

    seq_t result = pi_n_minus1 * (seq_t)n - pi_n * (seq_t)(n - 1);
    return result;
}

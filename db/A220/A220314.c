/*
 * A220314  binomial(2n,n) - (2n)^pi(n), where pi(n) is the number of primes
 *          <= n.
 * https://oeis.org/A220314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = binomial(2n,n) - (2n)^pi(n)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 16, i64 n <= 30, i128 n <= 65,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 480 (2026-11-01)
 * Helpers:   binomial, ipow, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n) {
    idx_t two_n = n * 2;
    seq_t bin = binomial(two_n, n);
    seq_t pi  = prime_pi(n);
    seq_t pow = ipow((seq_t)two_n, pi);
    return bin - pow;
}

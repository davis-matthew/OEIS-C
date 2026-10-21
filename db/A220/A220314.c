/*
 * A220314  binomial(2n,n) - (2n)^pi(n), where pi(n) is the number of primes
 *          <= n.
 * https://oeis.org/A220314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = binomial(2n,n) - (2n)^pi(n)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 14, i64 n <= 30, i128 n <= 64,
 *            big any n
 * Helpers:   big_pow, binomial, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/compute_pi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    idx_t two_n = n * 2;
    seq_t bin = binomial(two_n, n);
    seq_t pi  = compute_pi(n);
    seq_t pow = big_pow((seq_t)two_n, pi);
    return bin - pow;
}

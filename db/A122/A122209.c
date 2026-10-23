/*
 * A122209  Sum of squares of the first n^2 primes = A024450[n^2].
 * https://oeis.org/A122209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum[ Prime[k]^2, {k,1,n^2} ]
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* n >= 1; compute number of primes needed */
    idx_t m = n * n;          /* number of primes to sum over */
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k) {
        uint32_t p = get_prime(k);          /* kth prime */
        sum += (seq_t)p * (seq_t)p;          /* add its square */
    }
    return sum;
}

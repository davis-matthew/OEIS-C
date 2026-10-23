/*
 * A185269  The subsequence of primes, in order of occurrence, in A005351.
 * https://oeis.org/A185269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n-th prime value that arises from A005351, * i.e.,
 *            the n-th prime that is the binary number obtained by *
 *            interpreting the base(-2) representation of an integer as a *
 *            base(+2) number. The implementation searches k ≥ 1, maps k to *
 *            f(k) = Σ d_i 2^i where d_i are the base(-2) digits of...
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, map_k_to_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Important: include the helper that checks primality for arbitrary-precision
 * numbers. */
#include "../utils/is_prime_big.h"
#include <stdint.h>
#include <stdbool.h>
#include "../utils/map_k_to_f.h"

seq_t a(idx_t n)
{
    /* n is 1-based. */
    idx_t k = 1;       /* integer to evaluate */
    uint64_t found = 0;/* count of primes seen so far */

    for (;;) {
        seq_t val = map_k_to_f(k);
        if (is_prime_big(val)) {
            found++;
            if (found == n)
                return val;  /* the n-th prime value*/
        }
        k++;  /* move to the next integer */
    }
}

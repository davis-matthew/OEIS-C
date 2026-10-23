/*
 * A185145  Smallest average of twin prime pairs s such that n*s is also
 *          average of twin prime pairs.
 * https://oeis.org/A185145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest average s of twin prime pairs such that *
 *            n*s is also an average of twin prime pairs. * (Implementation:
 *            search over odd s, test primality conditions.)
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Offset: a(1) corresponds to the first average (4). */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    unsigned long long s = 4;   /* first candidate average (3,5) => 4 */
    for (;;) {
        /* Check if s is an average of a twin prime pair: (s-1, s+1) primes */
        if (is_prime_big((seq_t)(s - 1)) && is_prime_big((seq_t)(s + 1))) {
            seq_t ns = (seq_t)n * (seq_t)s;
            /* Verify that n*s is also an average of a twin prime pair */
            if (is_prime_big(ns - (seq_t)1) && is_prime_big(ns + (seq_t)1))
                return (seq_t)s;   /* found minimal s */
        }
        s += 2;   /* advance to next odd number */
    }
}

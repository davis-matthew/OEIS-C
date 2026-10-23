/*
 * A188385  Highest exponent in the prime factorization of n^n.
 * https://oeis.org/A188385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A051903(n), where A051903(n) = max exponent e with
 *            p^e dividing n (i.e., the largest prime exponent in the
 *            factorization of n). This is equivalent to the highest exponent
 *            in the prime factorization of n^n
 * Range:     u8 n <= 63, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _max_prime_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_max_prime_exponent.h"

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1, return 0 as safe default */

    uint64_t exp = _max_prime_exponent((uint64_t)n);
    return (seq_t)n * (seq_t)exp;      /* multiply in seq_t to avoid overflow in intermediate */
}

/*
 * A079068  Largest prime less than greatest prime factor of n but not
 *          dividing n, or 1 if no such prime exists.
 * https://oeis.org/A079068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 9031, u32 n <= 9031, u64 n <= 9031,
 *            u128 n <= 9031, big any n
 * Helpers:   is_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079068: a(n) = largest prime < gpf(n) not dividing n; a(1)=1 */

#include <stdbool.h>
#include "../utils/largest_prime_divisor.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t gpf = largest_prime_divisor(n);
    if (gpf <= 2) return (seq_t)1;

    long long p = (long long)gpf - 1;          // start just below the greatest prime factor
    for (; p >= 2; --p) {
        if (is_prime((idx_t)p) && (n % (idx_t)p != 0))
            return (seq_t)p;
    }
    return (seq_t)1;
}

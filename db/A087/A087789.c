/*
 * A087789  Smallest number greater than n having the same greatest prime
 *          factor as n; a(1)=1.
 * https://oeis.org/A087789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer m > n such that gpf(m) = gpf(n)
 *            (greatest prime factor). a(1) = 1. For n > 1 equals A071830(n)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   largest_prime_factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor_uint64.h"   /* provides seq_t largest_prime_factor_uint64(uint64_t) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t gpf_n = (uint64_t)largest_prime_factor_uint64((uint64_t)n);

    for (uint64_t m = n + 1; ; ++m) {
        uint64_t gpf_m = (uint64_t)largest_prime_factor_uint64(m);
        if (gpf_m == gpf_n)
            return (seq_t)m;
    }
}

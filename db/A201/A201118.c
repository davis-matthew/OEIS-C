/*
 * A201118  Primes of the form 3n^3+10.
 * https://oeis.org/A201118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 3*n^3+10
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t count = 0;
    for (uint64_t k = 1; ; ++k) {
        unsigned __int128 v128 = (unsigned __int128)3 * k * k * k + 10;
        if (v128 > UINT64_MAX) break; /* beyond 64-bit range */
        uint64_t v = (uint64_t)v128;
        if (is_prime(v)) {
            ++count;
            if (count == n)
                return (seq_t)v;
        }
    }
    return 0; /* failure case, should not happen for n within limits */
}

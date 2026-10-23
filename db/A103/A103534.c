/*
 * A103534  Concatenations of pairs of primes that differ by 1000.
 * https://oeis.org/A103534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Concatenate(P, P+1000) where P and P+1000 are primes
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   digits_uint64, is_prime, pow10_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/digits_uint64.h"
#include "../utils/pow10_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1, but return 0 */

    idx_t count = 0;
    for (idx_t p = 2; ; ++p) {
        if (!is_prime((uint64_t)p))
            continue;
        if (!is_prime((uint64_t)(p + 1000)))
            continue;

        ++count;
        if (count == n) {
            uint64_t q = p + 1000;
            int d = digits_uint64(q);
            seq_t mult = pow10_uint64(d);
            seq_t result = (seq_t)p * mult + (seq_t)q;
            return result;
        }
    }
}

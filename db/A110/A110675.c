/*
 * A110675  Numbers k such that the string 2222k is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A110675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A110675: a(n) is the n-th integer k such that 2222·10^{digits(k)} + k
   is a prime number.  The function searches candidates sequentially, testing
   each concatenated number for primality. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

static const uint64_t POW10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

static unsigned digits_uint64(uint64_t x)
{
    unsigned d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is offset 1, but we make a fallback for safety. */
        return (seq_t)0;
    }

    idx_t count = 0;
    uint64_t k = 1;

    for (;; ++k) {
        unsigned d = digits_uint64(k);
        uint64_t N = 2222ULL * POW10[d] + k;
        if (is_prime_uint64(N)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}

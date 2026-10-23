/*
 * A106766  Primes with digit sum = 29.
 * https://oeis.org/A106766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 283, u32 n <= 7900, u64 n <= 7900,
 *            u128 n <= 7900, big any n
 * Helpers:   _digit_sum_uint64, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose digit sum equals 29 (OEIS A106766) */
/* The value a(n) is the n-th such prime, with a(1)=2999. */
/* The implementation searches sequentially; it works for both C and C++ builds. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/_digit_sum_uint64.h"

seq_t a(idx_t n)
{
    /* Search the primes in increasing order until the n-th
       prime with digit sum 29 is found. */
    uint64_t count = 0;
    uint64_t candidate = 2;

    while (1) {
        if (is_prime_uint64(candidate)) {
            if (_digit_sum_uint64(candidate) == 29) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }

        /* advance to the next candidate; skip all even numbers after 2 */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}

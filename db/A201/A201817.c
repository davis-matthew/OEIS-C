/*
 * A201817  Numbers k such that 90*k + 67 is prime.
 * https://oeis.org/A201817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime_uint64, _mod_mul_uint64, _mod_pow_uint64,
 *            _small_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201817: numbers k such that 90*k + 67 is prime (search, no closed form) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_is_prime_uint64.h"
#include "../utils/_mod_mul_uint64.h"
#include "../utils/_mod_pow_uint64.h"
#include "../utils/_small_prime_trial.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 0;
    while (true) {
        uint64_t num = 90ULL * (uint64_t)k + 67ULL;
        if (_is_prime_uint64(num)) {
            ++found;
            if (found == n)
                return (seq_t)k;   /* cast to the build's seq_t type */
        }
        ++k;
    }
}

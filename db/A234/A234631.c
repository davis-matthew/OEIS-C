/*
 * A234631  Timestamps Hmmss where H,mm,ss are three consecutive primes, 0 < H
 *          < 24.
 * https://oeis.org/A234631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Timestamps Hmmss
   a(n) = (h*100 + m)*100 + s
   where (h,m,s) are three consecutive primes with h<24. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t cache[9];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        static const uint64_t primes[] = {
            2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
            13ULL, 17ULL, 19ULL, 23ULL
        };
        for (idx_t i = 0; i < 9; ++i) {
            uint64_t hour = primes[i];
            uint64_t minute = next_prime(hour);
            uint64_t second = next_prime(minute);
            uint64_t val = ((hour * 100ULL + minute) * 100ULL + second);
            cache[i] = (seq_t)val;
        }
        initialized = true;
    }

    if (n < 1 || n > 9) {
        /* Undefined for n beyond the finite definition; return 0. */
        return (seq_t)0;
    }
    return cache[n - 1];
}

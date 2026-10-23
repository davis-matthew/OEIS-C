/*
 * A182696  a(n) = the largest 4-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the largest 4‑digit number having exactly n divisors.
   a(n) is 0 for n > 64, because 64 is the maximum divisor count among
   four‑digit integers. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: divisor count for an unsigned 64‑bit integer. */
static idx_t divisor_count_u64(uint64_t n) {
    idx_t cnt = 1;
    uint64_t t = n;
    /* trial division by small primes (primes up to 97 cover all 4‑digit numbers). */
    static const uint64_t primes[25] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
        53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL,
        83ULL, 89ULL, 97ULL
    };
    for (size_t i = 0; i < 25 && primes[i] * primes[i] <= t; ++i) {
        if (t % primes[i] == 0) {
            uint64_t e = 0;
            do {
                t /= primes[i];
                ++e;
            } while (t % primes[i] == 0);
            cnt *= (e + 1);
        }
    }
    if (t > 1) {
        cnt *= 2;           /* remaining prime factor */
    }
    return cnt;
}

/* External linkage as required. */
seq_t a(idx_t n) {
    /* Cache largest 4‑digit number for each divisor count 0‑64. */
    static seq_t cache[65];
    static bool initialized = false;

    if (!initialized) {
        /* initialise cache to zero */
        for (size_t i = 0; i < 65; ++i) {
            cache[i] = 0;
        }
        /* scan all four‑digit numbers */
        for (uint64_t x = 1000; x <= 9999; ++x) {
            idx_t d = divisor_count_u64(x);
            if (d <= 64) {
                if (cache[d] < (seq_t)x) {
                    cache[d] = (seq_t)x;
                }
            }
        }
        initialized = true;
    }

    if (n <= 64) {
        return cache[(size_t)n];
    } else {
        return 0;
    }
}

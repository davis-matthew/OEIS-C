/*
 * A064964  100000n+1, 100000n+3, 100000n+7, 100000n+9 are all primes.
 * https://oeis.org/A064964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064964: a(n) = the n‑th integer k such that
   100000*k+1, 100000*k+3, 100000*k+7 and 100000*k+9 are all prime.
   The implementation searches candidates in order, caches all found
   values, and uses the provided is_prime_u64 routine for fast
   primality testing. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;   /* undefined; return 0 for safety */

    static uint64_t *cache64 = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t nextk = 1ull;

    if (cache_cnt < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t newcap = cache_cap ? cache_cap * 2 : 128;
            while (newcap < (size_t)n) newcap *= 2;
            uint64_t *tmp = (uint64_t *)realloc(cache64, newcap * sizeof(*tmp));
            if (!tmp) exit(EXIT_FAILURE);
            cache64 = tmp;
            cache_cap = newcap;
        }

        while (cache_cnt < (size_t)n) {
            if (nextk % 3ull != 1ull) { ++nextk; continue; }

            uint64_t m1 = nextk * 100000ull + 1ull;
            uint64_t m3 = nextk * 100000ull + 3ull;
            uint64_t m7 = nextk * 100000ull + 7ull;
            uint64_t m9 = nextk * 100000ull + 9ull;

            if (is_prime_u64(m1) && is_prime_u64(m3) &&
                is_prime_u64(m7) && is_prime_u64(m9)) {
                cache64[cache_cnt++] = nextk;
            }
            ++nextk;
        }
    }
    return (seq_t)cache64[n - 1];
}

/*
 * A171425  Divisors of 60^2 that are not divisors of 60.
 * https://oeis.org/A171425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Sequence A171425 – Divisors of 60² that are NOT divisors of 60.
 * The function returns the nth term (1‑based) of this finite increasing sequence.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for all terms (34 to be safe) */
    static seq_t   cache[34];
    static size_t  cache_len = 0;
    static bool    initialized = false;

    if (!initialized) {
        /* 60² = 2⁴ * 3² * 5²  */
        static const uint64_t p2[5] = {1ULL, 2ULL, 4ULL, 8ULL, 16ULL};
        static const uint64_t p3[3] = {1ULL, 3ULL, 9ULL};
        static const uint64_t p5[3] = {1ULL, 5ULL, 25ULL};

        const uint64_t base = 60ULL;      /* 60 = 2² * 3 * 5           */
        size_t idx = 0;

        for (uint64_t e2 = 0; e2 <= 4; ++e2)
            for (uint64_t e3 = 0; e3 <= 2; ++e3)
                for (uint64_t e5 = 0; e5 <= 2; ++e5) {
                    uint64_t val = p2[e2] * p3[e3] * p5[e5];
                    if (base % val != 0) {         /* not a divisor of 60  */
                        cache[idx++] = (seq_t)val;
                    }
                }

        /* sort ascending (simple insertion sort, at most 33 elements) */
        for (size_t i = 1; i < idx; ++i) {
            seq_t key = cache[i];
            size_t j = i;
            while (j > 0 && cache[j - 1] > key) {
                cache[j] = cache[j - 1];
                --j;
            }
            cache[j] = key;
        }

        cache_len = idx;
        initialized = true;
    }

    /* out‑of‑range indices return 0 (invalid)                     */
    if (n == 0)            return (seq_t)0;
    if (n > (idx_t)cache_len) return (seq_t)0;

    return cache[n - 1];
}

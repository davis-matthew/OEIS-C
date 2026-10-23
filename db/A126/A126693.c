/*
 * A126693  Prime numbers p such that 10000-p is also a prime.
 * https://oeis.org/A126693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   primes p such that 10000-p is also prime (OEIS A126693)
 * Range:     u8 n <= 8, u16 n <= 254, u32 n <= 254, u64 n <= 254,
 *            u128 n <= 254, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External linkage required */
seq_t a(idx_t n)
{
    /* The list is finite (254 terms).  We compute it once on the first call. */
    static seq_t prime_pairs[300];      /* room for all terms */
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t LIMIT = 10000ULL;   /* fixed sum 10000 */

        size_t idx = 0;
        for (idx_t p = 2; p < LIMIT; ++p) {
            if (is_prime(p) && is_prime(LIMIT - p)) {
                prime_pairs[idx++] = (seq_t)p;
            }
        }
        count = idx;
        initialized = true;
    }

    if (n == 0 || (size_t)n > count)
        return (seq_t)0;   /* undefined indices (allowed below offset) */

    return prime_pairs[(size_t)(n - 1)];
}

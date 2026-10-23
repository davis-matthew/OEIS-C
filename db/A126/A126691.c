/*
 * A126691  Prime numbers p such that 100-p is also a prime.
 * https://oeis.org/A126691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   compute primes p such that 100-p is also a prime by searching
 *            primes up to 100
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* precomputed (by algorithm) list of primes <=100 that pair with 100-p being prime */
    static seq_t list[12];
    static size_t len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* generate all primes up to 100, store those where 100-p is also prime */
        const idx_t LIMIT = 100;
        for (idx_t cand = 2; cand <= LIMIT; ++cand) {
            if (is_prime(cand)) {
                if (cand <= 100) {
                    idx_t partner = 100 - cand;
                    if (partner >= 2 && is_prime(partner)) {
                        list[len++] = (seq_t)cand;
                    }
                }
            }
        }
        initialized = true;
    }

    if (n >= 1 && (idx_t)n <= (idx_t)len) {
        return list[(size_t)(n - 1)];
    }
    /* For n beyond the finite number of terms, return 0 */
    return (seq_t)0;
}

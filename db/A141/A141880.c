/*
 * A141880  Primes congruent to 15 mod 19.
 * https://oeis.org/A141880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 15 (mod 19) – computed by
 *            searching for primes
 * Range:     u8 n <= 2, u16 n <= 355, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

/* Maximum number of terms we expect to be requested (≈2×10⁴ comfortably covers n ≈10⁴). */
#define MAX_TERMS 20000

/* External linkage function required by the build system. */
seq_t a(idx_t n)
{
    /* Static cache to store computed primes. */
    static seq_t  cache[MAX_TERMS];
    static idx_t  cache_size = 0;        /* Number of terms currently cached. */
    static idx_t  next_k    = 0;         /* k in the expression 19*k + 15, only even k produce odds. */

    /* Generate primes until we reach the requested index. */
    while (cache_size < n && cache_size < MAX_TERMS) {
        uint64_t cand = 19ULL * next_k + 15ULL;
        if (is_prime((idx_t)cand)) {
            cache[cache_size++] = (seq_t)cand;
        }
        next_k += 2;   /* Move to the next even k (odd k would give an even candidate). */
    }

    /* The sequence is defined for n >= 1; below that the behaviour is unspecified. */
    return cache[n - 1];
}

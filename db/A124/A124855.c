/*
 * A124855  Numbers k such that 3k + 4 and 4k + 3 are primes.
 * https://oeis.org/A124855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   append_candidate, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer k such that 3k+4 and 4k+3 are primes. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t count = 0, cap = 0;
    static idx_t next_k = 1;  /* next integer to test */

    /* Grow cache to accommodate n terms */
    while (count < (size_t)n) {
        idx_t k = next_k;
        /* Check 3k+4 and 4k+3 for primality */
        if (is_prime(3 * k + 4) && is_prime(4 * k + 3)) {
            /* Append k to cache */
            append_candidate(&cache, &count, &cap, (seq_t)k);
        }
        ++next_k;
    }

    return cache[n - 1];
}

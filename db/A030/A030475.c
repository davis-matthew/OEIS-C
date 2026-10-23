/*
 * A030475  Primes with property that all even digits occur together and all
 *          odd digits occur together.
 * https://oeis.org/A030475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime whose decimal digits contain at most one change
 *            of parity (the digits are grouped into a single block of evens
 *            followed by a single block of odds or vice‑versa). The function
 *            lazily generates the sequence by testing successive integers for
 *            primality and the parity‑block property,...
 * Range:     u8 n <= 45, u16 n <= 2222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_parity_block_property, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/has_parity_block_property.h"

static seq_t *cache = NULL;        /* memoised primes */
static size_t cache_size = 0;      /* number of stored primes */
static size_t cache_cap = 0;       /* allocated capacity */
static uint64_t next_candidate = 2;/* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (cache_size < (size_t)n) {
        while (true) {
            if (is_prime_small((seq_t)next_candidate) &&
                has_parity_block_property(next_candidate)) {
                if (cache_size == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* Allocation failed; return zero as a safe fallback. */
                        return 0;
                    }
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)next_candidate;
                ++next_candidate; /* advance for future searches */
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}

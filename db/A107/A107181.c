/*
 * A107181  Primes of the form 8x^2 + 9y^2.
 * https://oeis.org/A107181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes congruent to 17 (mod 24) that can be expressed as
 *            8x² + 9y²
 * Range:     u8 n <= 6, u16 n <= 820, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9240 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0, cache_len = 0;
static uint64_t candidate = 17;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 */

    while (cache_len < (size_t)n) {
        if (is_prime_u64(candidate)) {
            bool represent = false;
            uint64_t y_max = isqrt64(candidate) / 3;
            for (uint64_t y = 1; y <= y_max; ++y) {
                uint64_t y2 = y * y;
                uint64_t rem = candidate - 9 * y2;
                if (rem & 7)               /* rem % 8 != 0 */
                    continue;
                uint64_t x2 = rem >> 3;    /* rem / 8 */
                uint64_t x  = isqrt64(x2);
                if (x * x == x2) {
                    represent = true;
                    break;
                }
            }
            if (represent) {
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1U : 32;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp)
                        return 0; /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        candidate += 24; /* next number congruent to 17 mod 24 */
    }
    return cache[(size_t)n - 1];
}

/*
 * A175983  Squares n^2 that become prime after omitting all ones in their
 *          decimal expansion.
 * https://oeis.org/A175983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 12, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_big, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A175983: squares n^2 such that removing all decimal digits '1'
 * from n^2 leaves a prime. The function returns the nth such square
 * (a(1) = 121, a(2) = 1089, …).  Memoization is used so that repeated
 * calls are fast and order-independent.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_to_str.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* memoized terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_size = 0; /* number of cached terms */
static idx_t next_k = 1;      /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 – behaviour before it is irrelevant */

    if (n <= cache_size) return cache[n - 1];

    size_t found = cache_size;   /* count of cached qualifying squares */

    for (idx_t k = next_k; ; ++k) {
        seq_t square = (seq_t)k * k;

        /* obtain decimal representation of square */
        char buf[128];
        size_t len;
        seq_to_str(square, buf, &len);

        /* build number after deleting all '1' digits */
        seq_t trimmed = 0;
        for (size_t i = 0; i < len; ++i) {
            char c = buf[i];
            if (c == '1') continue;
            trimmed = trimmed * 10 + (seq_t)(c - '0');
        }

        if (trimmed > 1 && is_prime_big(trimmed)) {
            /* cache the term */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;        /* allocation failure fallback */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = square;
            ++found;
            if (found == n) {
                next_k = k + 1;
                return square;
            }
        }
    }
}

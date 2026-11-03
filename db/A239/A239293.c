/*
 * A239293  Smallest composite c > n such that n^c == n (mod c).
 * https://oeis.org/A239293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite c>n such that n^c == n (mod c)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9818 (2026-11-03)
 * Helpers:   is_prime_seq, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    ensure_cache(n, &cache, &cache_cap);     /* ensure cache size >= n+1 */

    if (cache[n] != 0) return cache[n];      /* memoised value */

    /* quick rule for even n : if n+1 is composite then a(n)=n+1 */
    if ((n & 1U) == 0 && !is_prime_seq((seq_t)(n + 1))) {
        cache[n] = (seq_t)(n + 1);
        return cache[n];
    }

    idx_t c = n + 1;
    for (;;) {
        if (!is_prime_seq((seq_t)c)) {      /* composite candidate */
            seq_t rem = powmod((seq_t)n, (uint64_t)c, (seq_t)c);
            if (rem == (seq_t)n % (seq_t)c) {
                cache[n] = (seq_t)c;
                return cache[n];
            }
        }
        ++c;
    }
}

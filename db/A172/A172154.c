/*
 * A172154  Numbers k such that 24*k+-5 are both prime.
 * https://oeis.org/A172154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence a(n): the n-th integer k such that both 24*k-5 and 24*k+5 are prime.
 * The function builds the list lazily and caches results for repeated calls.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Cache of found k values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static idx_t next_k = 1;       /* next candidate k to test */

    if (n == 0) {
        /* Undefined index; return 0 as a placeholder */
        return 0;
    }

    /* Grow cache until we have at least the n-th term */
    while (cache_len < (size_t)n) {
        /* Compute 24*k-5 and 24*k+5 */
        uint64_t p1 = 24ULL * (uint64_t)next_k - 5ULL;
        uint64_t p2 = 24ULL * (uint64_t)next_k + 5ULL;

        /* Check primality using the provided utility */
        if (is_prime((idx_t)p1) && is_prime((idx_t)p2)) {
            /* Allocate or grow cache as needed */
            if (cache_len == 0) {
                cache = mem_new(seq_t, 8);
                if (!cache) abort();
            } else {
                size_t new_cap = cache_len * 2;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}

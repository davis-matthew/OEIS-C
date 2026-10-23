/*
 * A045591  Numbers k that divide 6^k + 4^k.
 * https://oeis.org/A045591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth element of OEIS A045591.
   A number k is included iff k divides 6^k + 4^k.
   This implementation generates terms on demand and caches them
   for subsequent calls.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1U)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

static bool satisfies(uint64_t k)
{
    uint64_t m = k;
    uint64_t pow6 = modpow_uint64(6, m, m);
    uint64_t pow4 = modpow_uint64(4, m, m);
    uint64_t sum = pow6 + pow4;
    return (sum % m) == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;   /* next k to test */

    if (n == 0) return 0; /* undefined for n<1 */

    /* Expand cache until we have n terms */
    while (cache_len < (size_t)n) {
        for (; satisfies((uint64_t)next_k); ++next_k) {
            /* store the satisfying k */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                if (new_cap < (size_t)n) new_cap = (size_t)n;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); exit(EXIT_FAILURE); }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
            if (cache_len >= (size_t)n) break;
            ++next_k;
        }
        ++next_k; /* skip the last tested k that satisfied */
    }

    return cache[n - 1];
}

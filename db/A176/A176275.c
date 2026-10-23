/*
 * A176275  Numbers of the form 6k+1 with the least prime divisor of the form
 *          6m-1.
 * https://oeis.org/A176275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer of the form 6k+1 whose least prime divisor is
 *            6m-1
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>


static uint64_t _spf_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    if (n % 3ULL == 0ULL) return 3ULL;
    for (uint64_t i = 5ULL; i * i <= n; i += 6ULL) {
        if (n % i == 0ULL)        return i;
        if (n % (i + 2ULL) == 0ULL) return i + 2ULL;
    }
    return n; /* n is prime */
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 7ULL; /* first number ≡1 (mod 6) greater than 1 */

static void _append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    while (cache_len < n) {
        uint64_t lpf = _spf_uint64(next_candidate);
        if (lpf % 6ULL == 5ULL) /* least prime divisor ≡ 5 (mod 6) */
            _append_term((seq_t)next_candidate);
        next_candidate += 6ULL;
    }
    return cache[n - 1];
}

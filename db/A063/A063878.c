/*
 * A063878  Duplicate of A063167.
 * https://oeis.org/A063878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th composite number m whose base-8 representation
 *            contains the base-8 representation of its largest proper factor
 *            m/spf(m) as a substring (search)
 * Range:     i8 none, i16 n <= 5, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void A063878_to_octal(uint64_t n, char *buf, size_t *len)
{
    size_t i = 0;
    do {
        buf[i++] = (char)('0' + (n & 7));
        n >>= 3;
    } while (n);
    *len = i;
    if (i > 1) {
        for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
            char tmp = buf[l];
            buf[l] = buf[r];
            buf[r] = tmp;
        }
    }
}

static bool A063878_contains_octal(uint64_t big, uint64_t small)
{
    char buf_big[32], buf_small[32];
    size_t len_big, len_small;

    A063878_to_octal(big, buf_big, &len_big);
    A063878_to_octal(small, buf_small, &len_small);

    if (len_small > len_big) return false;

    for (size_t i = 0; i + len_small <= len_big; ++i) {
        size_t j;
        for (j = 0; j < len_small; ++j) {
            if (buf_big[i + j] != buf_small[j]) break;
        }
        if (j == len_small) return true;
    }
    return false;
}

static uint64_t A063878_spf(uint64_t n)
{
    if (n <= 3) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if ((n % p) == 0) return p;
    }
    return n;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_done = 3;

    if (n == 0) return 0;

    while (cache_len < (size_t)n) {
        ++last_done;
        uint64_t spf = A063878_spf(last_done);
        if (spf == last_done) continue;

        uint64_t largest = last_done / spf;
        if (!A063878_contains_octal(last_done, largest)) continue;

        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *new_cache = mem_new(seq_t, new_cap);
            if (!new_cache) abort();
            if (cache) mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)last_done;
    }

    return cache[n - 1];
}

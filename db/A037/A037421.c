/*
 * A037421  Positive numbers having the same set of digits in base 3 and base
 *          9.
 * https://oeis.org/A037421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer with equal digit set in base 3 and
 *            base 9
 * Range:     u8 n <= 14, u16 n <= 263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9996 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static inline bool digit_set_equal(seq_t val, uint8_t mask9)
{
    uint8_t mask3 = 0;
    seq_t x = val;
    while (x) {
        uint8_t d = (uint8_t)(x % 3);
        mask3 |= 1u << d;
        x /= 3;
    }
    return mask3 == mask9;
}

static void append_value(seq_t val, seq_t **cache, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < *len; ++i)
            tmp[i] = (*cache)[i];
        if (*cache)
            free_seq_array(*cache);
        *cache = tmp;
        *cap = new_cap;
    }
    (*cache)[(*len)++] = val;
}

static void gen_rec(int pos, int length, seq_t val, uint8_t mask9,
                    seq_t **cache, size_t *len, size_t *cap)
{
    if (pos == length) {
        if (digit_set_equal(val, mask9))
            append_value(val, cache, len, cap);
        return;
    }
    int start = (pos == 0) ? 1 : 0;
    for (int d = start; d <= 2; ++d) {
        gen_rec(pos + 1, length,
                val * (seq_t)9 + (seq_t)d,
                mask9 | (1u << d),
                cache, len, cap);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static int max_generated_len = 0;

    while (cache_len < (size_t)n) {
        ++max_generated_len;
        gen_rec(0, max_generated_len, (seq_t)0, 0,
                &cache, &cache_len, &cache_cap);
    }

    return cache[n - 1];
}

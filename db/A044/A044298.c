/*
 * A044298  Numbers k such that the string 5,2 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044298: Numbers k such that the string 5,2 occurs in the
 * base‑9 representation of k but not of k-1.  Computed by a linear scan
 * with memoisation. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static inline bool has52(seq_t x)
{
    seq_t prev = x % 9;
    x /= 9;
    while (x > 0)
    {
        seq_t cur = x % 9;
        if (cur == 5 && prev == 2) return true;
        prev = cur;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_test = 1;

    if (cache == NULL)
    {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
    }

    while (cache_len < n)
    {
        while (true)
        {
            if (has52(next_test))
            {
                seq_t prev = next_test - 1;
                if (!has52(prev))
                {
                    if (cache_len == cache_cap)
                    {
                        size_t new_cap = cache_cap * 2;
                        seq_t *new_cache = alloc_seq_array(new_cap);
                        for (size_t i = 0; i < cache_len; ++i)
                            new_cache[i] = cache[i];
                        free_seq_array(cache);
                        cache = new_cache;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = next_test;
                    ++next_test;
                    break;
                }
            }
            ++next_test;
        }
    }

    return cache[n - 1];
}

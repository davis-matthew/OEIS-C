/*
 * A044758  Numbers n such that string 4,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th positive integer k such that the decimal
 *            representation of k contains the substring "45" but the
 *            representation of k+1 does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Return true if the decimal representation of x contains "45". */
static bool has45(seq_t x)
{
    seq_t prev = 10;            /* placeholder for previous digit */
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 4 && prev == 5)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for offset 0; harmless */

#if defined(__cplusplus)
    /* C++ build: use std::vector for automatic memory management. */
    static std::vector<seq_t> cache;
    static seq_t last_checked = 0;

    while (cache.size() < (size_t)n) {
        ++last_checked;
        if (has45(last_checked) && !has45(last_checked + 1))
            cache.push_back(last_checked);
    }
    return cache[n - 1];

#else
    /* C build: manual reallocation of a seq_t array. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static seq_t last_checked = 0;

    while (cache_size < (size_t)n) {
        ++last_checked;
        if (has45(last_checked) && !has45(last_checked + 1)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)              /* allocation failure */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }
    return cache[n - 1];
#endif
}

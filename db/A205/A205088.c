/*
 * A205088  a(n) = n 8's sandwiched between two 1's.
 * https://oeis.org/A205088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, base
 * Formula:   a(n) = 10*a(n-1) + 71, a(0) = 11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t last = 0;

    size_t idx = (size_t)n;

    if (!cache) {
        cache = (seq_t *)malloc((idx + 1) * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)11;
        cap = idx + 1;
        last = 0;
    } else if (idx + 1 > cap) {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    if (idx > last) {
        for (size_t i = last + 1; i <= idx; ++i) {
            cache[i] = cache[i - 1] * (seq_t)10 + (seq_t)71;
        }
        last = idx;
    }

    return cache[idx];
}

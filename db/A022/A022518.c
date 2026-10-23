/*
 * A022518  Describe previous term from the right (method B - initial term is
 *          7).
 * https://oeis.org/A022518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A022518 – Describe previous term from the right (method B, initial term 7)
 * a(0) = 7
 * For n > 0: a(n) is obtained by taking a(n‑1), scanning its decimal digits from
 * right to left, grouping identical digits, and for each group writing the
 * digit followed by the frequency of that group. The groups are written in the
 * order seen from right to left, producing a new decimal integer.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static void append_count(seq_t *acc, idx_t count)
{
    uint8_t digs[25];
    size_t l = 0;
    do {
        digs[l++] = count % 10;
        count /= 10;
    } while (count);
    while (l--) {
        *acc = *acc * 10 + digs[l];
    }
}

static seq_t describe(seq_t prev)
{
    seq_t res = 0;
    while (prev > 0) {
        seq_t dig = prev % 10;
        idx_t  cnt = 0;
        while (prev > 0 && (prev % 10) == dig) {
            ++cnt;
            prev /= 10;
        }
        res = res * 10 + dig;
        append_count(&res, cnt);
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    if (n >= cache_size) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = cache_size; i <= n; ++i) {
            if (i == 0) cache[i] = 7;
            else        cache[i] = describe(cache[i - 1]);
        }
        cache_cap   = new_cap;
        cache_size = n + 1;
    }
    return cache[n];
}

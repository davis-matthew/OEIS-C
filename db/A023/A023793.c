/*
 * A023793  Katadromes: digits in base 12 are in strict descending order.
 * https://oeis.org/A023793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 3337, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   gen_recursive
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A023793: the nth smallest integer whose base‑12 representation
 * has digits in strict descending order. The function generates all such
 * numbers (there are 2¹²−1 = 4095 of them), sorts them, and returns the
 * requested term. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gen_recursive.h"

static int seq_compare(const void *p, const void *q)
{
    const seq_t *x = (const seq_t *)p;
    const seq_t *y = (const seq_t *)q;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void build_cache(seq_t *buf, size_t *cnt)
{
    size_t c = 0;
    for (int start = 11; start >= 0; --start) {
        gen_recursive(start, (seq_t)start, buf, &c);
    }
    *cnt = c;
    qsort(buf, c, sizeof(seq_t), seq_compare);
}

seq_t a(idx_t n)
{
    /* static cache of all sequence values (generated once) */
    static seq_t cache[4096];
    static size_t cache_size = 0;

    if (cache_size == 0) {
        build_cache(cache, &cache_size);
    }
    if (n == 0 || n > cache_size) {
        return cache[cache_size - 1];
    }
    return cache[n - 1];
}

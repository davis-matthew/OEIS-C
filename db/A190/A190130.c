/*
 * A190130  Numbers 1 through 10000 sorted lexicographically in octal
 *          representation (base 8).
 * https://oeis.org/A190130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth number in 1..10000 sorted lexicographically by
 *            its octal (base‑8) representation
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: full, pass, 10000 terms through n = 10000 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/mem.h"

static int oct_cmp(const void *a, const void *b)
{
    idx_t ia = *(const idx_t *)a;
    idx_t ib = *(const idx_t *)b;
    char sa[16];
    char sb[16];
    sprintf(sa, "%o", (unsigned)ia);
    sprintf(sb, "%o", (unsigned)ib);
    return strcmp(sa, sb);
}

static seq_t *cache = NULL;
static const idx_t CACHE_SIZE = 10000;

seq_t a(idx_t n)
{
    if (!cache)
    {
        cache = mem_new(seq_t, CACHE_SIZE);
        if (!cache) return 0;  /* allocation failure */
        idx_t *order = (idx_t *)malloc(CACHE_SIZE * sizeof(idx_t));
        if (!order)
        {
            mem_free(cache);
            return 0;
        }
        for (idx_t i = 0; i < CACHE_SIZE; ++i) order[i] = i + 1;
        qsort(order, CACHE_SIZE, sizeof(idx_t), oct_cmp);
        for (idx_t i = 0; i < CACHE_SIZE; ++i) cache[i] = order[i];
        free(order);
    }
    if (n < 1 || n > CACHE_SIZE) return 0;  /* out of defined range */
    return cache[n - 1];
}

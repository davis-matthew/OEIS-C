/*
 * A164304  a(n) = 4*a(n-1) - 2*a(n-2) for n > 1; a(0) = 3, a(1) = 14.
 * https://oeis.org/A164304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) (a(0)=3, a(1)=14)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 814 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cached_size = 0; /* number of elements stored in cache */

/* Ensure cache holds values up to index n inclusive. */
static void ensure(idx_t n)
{
    if (cache == NULL) {
        cache = mem_new(seq_t, ((n + 1) * sizeof *cache) / sizeof(seq_t));
        cached_size = n + 1;
        if (n >= 0)  cache[0] = (seq_t)3;
        if (n >= 1)  cache[1] = (seq_t)14;
        for (idx_t i = 2; i <= n; ++i) {
            cache[i] = ((seq_t)4 * cache[i - 1]) - ((seq_t)2 * cache[i - 2]);
        }
    } else if (n >= cached_size) {
        seq_t *tmp = mem_resize(seq_t, cache, ((n + 1) * sizeof *cache) / sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cached_size; i <= n; ++i) {
            cache[i] = ((seq_t)4 * cache[i - 1]) - ((seq_t)2 * cache[i - 2]);
        }
        cached_size = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)14;
    ensure(n);
    return cache[n];
}

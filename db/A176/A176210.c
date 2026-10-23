/*
 * A176210  Triangle read by rows in which row n (n>=3) lists those partitions
 *          of n with every part > 2.
 * https://oeis.org/A176210
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the n‑th element (0‑based offset 3) of the list obtained
 *            by concatenating, in decreasing lexicographic order, all
 *            partitions of numbers k (k ≥ 3) whose parts are all > 2
 * Range:     u8 n <= 9853, u16 n <= 9853, u32 n <= 9853, u64 n <= 9853,
 *            u128 n <= 9853, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;        /* flattened sequence of parts */
static size_t cache_size = 0;      /* current number of stored parts */
static size_t cache_capacity = 0;  /* current buffer capacity */

static idx_t next_row = 3;         /* next row to generate (k ≥ 3) */

/* Append a value to the cache, reallocating if necessary. */
static void cache_push(seq_t val)
{
    if (cache_size == cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }
    cache[cache_size++] = val;
}

/* Recursive generation of partitions of ‘rem’ with each part ≤ max_part.
   For each finished partition, its parts are appended to the cache. */
static void gen_partitions(idx_t rem, idx_t max_part, int *parts, int depth)
{
    if (rem == 0) {
        for (int i = 0; i < depth; ++i)
            cache_push((seq_t)parts[i]);
        return;
    }

    for (idx_t part = (rem < max_part ? rem : max_part); part >= 3; --part) {
        parts[depth] = (int)part;
        gen_partitions(rem - part, part, parts, depth + 1);
    }
}

/* Generate rows until the cache contains at least target+1 entries. */
static void ensure_cache(idx_t target)
{
    static int parts[5000]; /* big enough for any partition needed */
    while (cache_size <= target) {
        gen_partitions(next_row, next_row, parts, 0);
        ++next_row;
    }
}

/* Main function. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;         /* undefined for n < 3 */

    /* n = 3 → index 0, so index = n-3. */
    idx_t pos = n - 3;

    if (cache == NULL) {         /* lazy allocation of cache */
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache_size = 0;
    }

    ensure_cache(pos);
    return cache[pos];
}

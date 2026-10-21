/*
 * A214333  Trajectory of 1 under evenly many applications of the morphism 1
 *          -> 2, 2 -> 114, 3 -> 4, 4 -> 233.
 * https://oeis.org/A214333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol of the fixed point of the morphism σ
 *            defined on {1,4} by σ(1) = 114 σ(4) = 11444 The sequence is
 *            obtained by starting with "1" and repeatedly applying σ, keeping
 *            the first n+1 symbols
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

static seq_t *cache = NULL;     /* cached word */
static size_t cache_len = 0;    /* number of symbols in cache */

/* Expand the cached word until it contains at least n+1 symbols.
   The expansion follows σ: 1 → 1,1,4 ; 4 → 1,1,4,4,4. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t*)malloc(1 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Expand while not enough symbols */
    while (cache_len <= (size_t)n) {
        /* First compute length of next expansion */
        size_t new_len = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)4)
                new_len += 5;   /* σ(4) has length 5 */
            else
                new_len += 3;   /* σ(1) has length 3 */
        }

        /* Allocate new buffer */
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));

        /* Perform expansion */
        size_t pos = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)1) {
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)4;
            } else { /* cache[i] == 4 */
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)4;
                new_cache[pos++] = (seq_t)4;
                new_cache[pos++] = (seq_t)4;
            }
        }

        /* Replace old cache */
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

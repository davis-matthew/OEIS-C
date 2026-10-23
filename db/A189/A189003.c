/*
 * A189003  Number of domino tilings of the 5 X n grid with upper left corner
 *          removed iff n is odd.
 * https://oeis.org/A189003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 15*a(n-2) – 32*a(n-4) + 15*a(n-6) – a(n-8)
   Initial values: a(0)=1, a(1)=1, a(2)=8, a(3)=15, a(4)=95, a(5)=192, a(6)=1183, a(7)=2415 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Initial 8 terms */
static const seq_t init_terms[8] = {
    (seq_t)1, (seq_t)1, (seq_t)8,
    (seq_t)15, (seq_t)95, (seq_t)192,
    (seq_t)1183, (seq_t)2415
};

/* Extend cache to at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    /* If realloc fails, we simply return and use the old cache – the
       caller will eventually produce an undefined value, which is
       acceptable for an out-of-memory situation. */
    cache = new_cache;
    /* Initialise newly allocated entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init_terms[i];
        } else {
            seq_t a2 = cache[i - 2];
            seq_t a4 = cache[i - 4];
            seq_t a6 = cache[i - 6];
            seq_t a8 = cache[i - 8];

            /* a(n) = 15*a(n-2) – 32*a(n-4) + 15*a(n-6) – a(n-8) */
            seq_t next = a2 * (seq_t)15
                        - a4 * (seq_t)32
                        + a6 * (seq_t)15
                        - a8;
            cache[i] = next;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (cache_len == 0) {
        /* initialise cache with the first 8 terms */
        cache = (seq_t*)malloc(8 * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_terms[i];
        cache_len = 8;
    }

    if (n < cache_len)
        return cache[n];

    ensure_cache(n);
    return cache[n];
}

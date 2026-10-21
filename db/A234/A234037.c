/*
 * A234037  The union of odious numbers with evil squares and evil numbers
 *          with odious squares.
 * https://oeis.org/A234037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 838, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that popcount(n)+popcount(n^2) is odd (OEIS A234037) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Count the binary weight of a seq_t value */
static unsigned int popcount_seq(seq_t v)
{
    unsigned int cnt = 0;
    while (v != 0) {
        cnt += (unsigned int)(v & 1);
        v >>= 1;
    }
    return cnt;
}

/* Return the nth element of A234037, 1‑based */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t last_checked = 0; /* last integer examined */

    /* Resize cache if necessary */
    if (cache_len < n && cache_cap < n) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate elements until we have the nth one */
    while (cache_len < n) {
        seq_t x = ++last_checked;
        seq_t sq = x * x;
        unsigned int pc1 = popcount_seq(x);
        unsigned int pc2 = popcount_seq(sq);
        if (((pc1 + pc2) & 1) != 0) { /* odd sum of weights */
            cache[cache_len++] = x;
        }
    }

    return cache[n - 1];
}

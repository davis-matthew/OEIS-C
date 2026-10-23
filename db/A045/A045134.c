/*
 * A045134  Numbers whose base-4 representation contains no 2's and exactly
 *          one 3.
 * https://oeis.org/A045134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A045134: Numbers whose base-4 representation contains no 2's and exactly one 3.
 *
 * This implementation builds the sequence by scanning integers in increasing
 * order and selecting those whose base‑4 digits satisfy the condition.
 * The results are cached so that repeated calls are efficient.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool valid_number(seq_t x) {
    int cnt3 = 0;
    while (x > 0) {
        int digit = (int)(x % 4);
        if (digit == 2) return false;
        if (digit == 3) {
            cnt3++;
            if (cnt3 > 1) return false;
        }
        x /= 4;
    }
    return cnt3 == 1;
}

seq_t a(idx_t n) {
    /* Handle offset 1: a(1) is the first term. */
    if (n == 0) return 0;  /* undefined, but safe */

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static seq_t  current = 3;          /* last examined number */

    /* First call: allocate initial cache */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)3;            /* a(1) */
        cache_len = 1;
        current = 3;
    }

    /* Generate terms until we have at least n terms */
    while (cache_len < n) {
        /* Find next valid number */
        do {
            current++;
        } while (!valid_number(current));

        /* Grow cache if required */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = current;
    }

    return cache[n - 1];
}

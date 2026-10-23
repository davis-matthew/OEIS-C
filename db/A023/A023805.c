/*
 * A023805  Xenodromes: all digits in base 11 are different.
 * https://oeis.org/A023805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   distinct_base11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Description: Computes A023805 – numbers whose base‑11 representation contains no repeated digits.
 * The function generates the sequence in increasing order by checking integers one by one
 * and keeping a static cache so that repeated calls are fast.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/distinct_base11.h"

static seq_t *cache = NULL;      /* cache of generated terms */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_cnt = 0;     /* number of stored terms */
static seq_t next_scan = 1;      /* next integer to test after the last cached term */

/* ensure we have generated at least n terms (n >= 1) */
static void generate_up_to(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache_cnt = 0;
    }

    /* first term a(1) = 0 */
    if (cache_cnt == 0) {
        cache[0] = (seq_t)0;
        cache_cnt = 1;
        next_scan = 1;
    }

    while (cache_cnt < (size_t)n) {
        seq_t candidate = next_scan;
        next_scan++;

        if (distinct_base11(candidate)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* out of memory – terminate (undefined behaviour for this assignment) */
                    return;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, but harmless */
    generate_up_to(n);
    return cache[n - 1];
}

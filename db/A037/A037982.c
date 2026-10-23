/*
 * A037982  Numbers whose maximal base 5 run length is 3.
 * https://oeis.org/A037982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = n-th positive integer whose base-5 representation has maximal
 * run length exactly 3 (i.e., it contains a run of three identical digits
 * but no run of four or more identical digits).
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: true if num's base-5 representation has maximal run length == 3 */
static inline bool has_max_run3(seq_t num)
{
    if (num == 0) return false;
    int max_run = 0;
    int curr_run = 0;
    int prev_digit = 5;            /* sentinel value not in [0,4] */
    while (num > 0) {
        int digit = (int)(num % 5);
        num   /= 5;
        if (digit == prev_digit) {
            curr_run++;
        } else {
            if (curr_run > max_run) max_run = curr_run;
            curr_run = 1;
            prev_digit = digit;
        }
    }
    if (curr_run > max_run) max_run = curr_run;
    return max_run == 3;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache: nth term is cache[n-1] */
    static seq_t  *cache = NULL;
    static size_t  cache_sz = 0;
    static size_t  cache_cap = 0;
    static seq_t   last_val = 0;

    /* Return from cache if already computed */
    if (n <= cache_sz) return cache[n-1];

    /* Initialize cache on first use */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
    }

    /* Generate terms until we have at least n of them */
    while (cache_sz < n) {
        seq_t cand = last_val + 1;
        /* Search forward for next valid value */
        while (!has_max_run3(cand)) cand++;

        /* Grow cache if necessary */
        if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = cand;
        last_val = cand;
    }

    return cache[n-1];
}

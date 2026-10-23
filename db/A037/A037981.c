/*
 * A037981  Maximal base 5 run length is 2.
 * https://oeis.org/A037981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑5 representation
 *            contains a maximal run of identical digits of length exactly two
 *            (i.e., at least one run of length two and no run of length three
 *            or more)
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms stored */
    static size_t cap = 0;      /* allocated capacity */
    static idx_t next_candidate = 1; /* next integer to test */

    /* Allocate initial cache */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cached < n) {
        /* Test whether next_candidate satisfies the run‑length condition */
        idx_t temp = next_candidate;
        int prev_digit = -1;
        int cur_run = 0;
        int max_run = 0;
        while (temp > 0) {
            int digit = (int)(temp % 5);
            if (digit == prev_digit) {
                cur_run++;
            } else {
                prev_digit = digit;
                cur_run = 1;
            }
            if (cur_run > max_run) max_run = cur_run;
            if (max_run > 2) break;          /* not allowed */
            temp /= 5;
        }

        /* Candidate accepted if max_run == 2 */
        if (max_run == 2) {
            /* Ensure capacity */
            if (cached + 1 > cap) {
                size_t new_cap = cap * 2;
                if (new_cap == 0) new_cap = 1;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            cache[cached++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return cache[n - 1];
}

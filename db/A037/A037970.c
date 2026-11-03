/*
 * A037970  Numbers whose maximal base-2 run length is 3.
 * https://oeis.org/A037970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9999 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer whose maximal consecutive run length
   in binary is exactly 3. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the maximum run length of consecutive equal bits in
   the binary representation of x equals 3. */
static inline bool has_max_run3(uint64_t x)
{
    if (x == 0)
        return false;           /* not a positive integer */

    unsigned int max_run = 0;
    unsigned int run_len = 0;
    int current_bit = 0;

    /* Process least‑significant bit first */
    current_bit = (int)(x & 1u);
    run_len = 1;
    x >>= 1;

    while (x != 0) {
        int bit = (int)(x & 1u);
        if (bit == current_bit) {
            run_len++;
        } else {
            if (run_len > max_run)
                max_run = run_len;
            current_bit = bit;
            run_len = 1;
        }
        x >>= 1;
    }

    /* last run */
    if (run_len > max_run)
        max_run = run_len;

    return max_run == 3;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t next_candidate = 1;  /* next integer to test */

    /* Ensure that we can store at least n terms */
    while (cache_len < n) {
        /* Test whether the current candidate satisfies the condition */
        if (has_max_run3(next_candidate)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) {
                    /* Allocation failure – terminate */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    /* n is 1‑based; indices in C are 0‑based */
    return cache[n - 1];
}

/*
 * A044813  Positive integers having distinct base-2 run lengths.
 * https://oeis.org/A044813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1418, u32 n <= 8681, u64 n <= 8681,
 *            u128 n <= 8681, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044813 – positive integers having distinct base-2 run lengths.
   Computation: brute‑force search testing each integer until the required
   index is reached; results are cached so that calls are fast and may
   arrive in any order. */
#include <stdbool.h>
#include <stdlib.h>

/* Maximum run length we are willing to inspect.  The only integers that
   ever have longer runs in the generated values are far beyond the
   size of any seq_t that fits in practice.  For safety we treat any
   longer run as a failure. */
#define MAX_RUN_LENGTH 199

/* Return true iff every run of consecutive equal bits in the binary
   representation of m has a distinct length.  */
static bool has_distinct_run_lengths(seq_t m)
{
    if (m == 0)
        return false;                /* 0 never appears */

    unsigned char used[MAX_RUN_LENGTH + 1] = {0};

    seq_t v = m;
    int prev_bit = -1;
    int run = 0;

    while (v) {
        int bit = (int)(v & (seq_t)1);
        if (prev_bit == -1) {               /* first bit */
            prev_bit = bit;
            run = 1;
        } else if (bit == prev_bit) {       /* same run */
            run++;
        } else {                            /* run finished */
            if (run > MAX_RUN_LENGTH)
                return false;
            if (used[run])
                return false;
            used[run] = 1;
            prev_bit = bit;
            run = 1;
        }
        v >>= 1;
    }

    /* Handle the final run. */
    if (run > MAX_RUN_LENGTH)
        return false;
    if (used[run])
        return false;
    return true;
}

/* Return the n‑th term of A044813 (a(1) is the first term).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1, but this is harmless */

    /* Static cache so that calls may arrive in arbitrary order. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_val = 0;          /* last integer tested */

    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = (cache_cap < 64) ? 64 : cache_cap * 2;
            if (new_cap < (size_t)n)
                new_cap = n;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        while (cache_len < (size_t)n) {
            last_val++;                            /* next positive integer */
            if (has_distinct_run_lengths(last_val))
                cache[cache_len++] = last_val;
        }
    }
    return cache[(size_t)n - 1];
}

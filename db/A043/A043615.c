/*
 * A043615  Numbers whose base-6 representation has exactly 7 runs.
 * https://oeis.org/A043615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043615: Numbers whose base-6 representation has exactly 7 runs */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Internal cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms stored */
    static seq_t next_value = 46656; /* start scanning from 6^6 */

    if (n == 0) /* offset is 1, but guard against invalid request */
        return (seq_t)0;

    /* Grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Ensure capacity for at least one more term */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8192;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next number with exactly 7 runs in base-6 */
        for (seq_t x = next_value;; ++x) {
            size_t runs = 1;
            seq_t t = x;
            int prev = -1;

            /* Count runs while processing each digit */
            while (t) {
                int digit = (int)(t % (seq_t)6);
                t /= (seq_t)6;

                if (prev == -1) {
                    prev = digit;
                    continue;
                }

                if (digit != prev) {
                    ++runs;
                    if (runs > 7)   /* early exit if too many runs */
                        break;
                }
                prev = digit;
            }

            if (t == 0 && runs == 7) {
                /* Record found term */
                cache[cache_len++] = x;
                next_value = x + (seq_t)1; /* continue search after this number */
                break;
            }
        }
    }

    return cache[n - 1];
}

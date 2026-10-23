/*
 * A043370  Numbers having two 0's in base 6.
 * https://oeis.org/A043370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers having exactly two zeros in base-6 representation
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_num = 0;

    if (n == 0) return (seq_t)0;  /* safety for zero index */

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        idx_t x = next_num;
        int zeros = 0;

        if (x == 0) {
            /* base‑6 representation of 0 is "0" -> one zero */
            zeros = 1;
        } else {
            while (x > 0) {
                if ((x % 6) == 0) {
                    zeros++;
                    if (zeros > 2) break;  /* early exit */
                }
                x /= 6;
            }
        }

        if (zeros == 2) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 256;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – return 0 (should never happen in tests) */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        next_num++;
    }
    return cache[n - 1];
}

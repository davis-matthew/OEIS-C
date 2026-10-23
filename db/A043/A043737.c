/*
 * A043737  Numbers whose number of runs in the base-2 representation is
 *          congruent to 4 mod 6.
 * https://oeis.org/A043737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: brute‑force search of numbers whose number of binary runs is ≡ 4 mod 6 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n = 0 is outside the OEIS offset; returning 0 is harmless */
    if (n == 0)
        return 0;

    static seq_t *cache     = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_checked = 0;

    while (cache_size < n) {
        idx_t num = last_checked + 1;
        last_checked = num;

        /* Count runs in the binary representation of num */
        idx_t m = num;
        unsigned int runs = 0;
        if (m != 0) {
            int prev_bit = m & 1;
            m >>= 1;
            while (m) {
                int bit = m & 1;
                if (bit != prev_bit) {
                    runs++;
                    prev_bit = bit;
                }
                m >>= 1;
            }
            runs++; /* account for the first run */
        } else {
            runs = 1; /* representation of 0 */
        }

        if (runs % 6 == 4) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);  /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)num;
        }
    }

    return cache[n - 1];
}

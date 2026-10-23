/*
 * A173639  Numbers n such that the sum of the digits of 11*n is odd.
 * https://oeis.org/A173639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers n such that the digit sum of 11*n is odd
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t last_checked = 0;

    if (n == 0)
        return 0; /* undefined for n=0 */

    if (n <= len)
        return cache[n - 1];

    while (len < n) {
        last_checked++;

        seq_t prod = ((seq_t)last_checked) * 11;
        seq_t sum = digit_sum(prod);
        if ((sum & 1) != 0) {
            if (len >= cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *new_cache =
                    (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache)
                    exit(1); /* allocation failure */
                cache = new_cache;
                cap = new_cap;
            }
            cache[len++] = last_checked;
        }
    }
    return cache[n - 1];
}

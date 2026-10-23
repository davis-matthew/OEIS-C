/*
 * A038178  Numbers k such that k = (sum of digits of k)^(number of digits of
 *          k).
 * https://oeis.org/A038178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, fini, full, base
 * Formula:   a(n) = k such that k = (sum of digits of k)^(number of digits of
 *            k)
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, digit_len, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_len.h"
#include "../utils/digit_sum.h"

/* Returns the n-th element of sequence A038178.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* Largest digit count that has already been processed */
    static idx_t last_processed_d = 0;

    if (n == 0)
        return 0;                 /* undefined for 0 */

    /* Generate terms until we have at least n of them or reach safety limit */
    while (cache_size < (size_t)n && last_processed_d < 200) {
        ++last_processed_d;      /* current digit count d */
        idx_t d = last_processed_d;
        idx_t s_start = (d == 1 ? 0 : 1);
        idx_t s_end   = 9 * d;

        for (idx_t s = s_start; s <= s_end; ++s) {
            seq_t power = big_pow((seq_t)s, (seq_t)d);

            /* Skip values that do not have exactly d digits */
            if (digit_len(power) != (int)d)
                continue;

            /* Check that digit sum equals the base s */
            if (digit_sum(power) != (seq_t)s)
                continue;

            /* Store the valid term, enlarging the cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = power;
        }
    }

    /* Return the requested term if available */
    if (n <= cache_size)
        return cache[n - 1];

    /* Out of range – sequence ends here */
    return 0;
}

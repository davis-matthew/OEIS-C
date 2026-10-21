/*
 * A069029  Powers of 4 with strictly increasing sum of digits.
 * https://oeis.org/A069029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069029: Powers of 4 with strictly increasing decimal digit sums.
   a(1) = 1, a(2) = 4, a(3) = 16, a(4) = 64, …
   For each n ≥ 1, a(n) is the n‑th power of 4 for which the digit sum
   is strictly larger than that of the previous term. */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; indices < 1 are irrelevant */

    /* static memoization state */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cached_terms = 0;
    static seq_t last_power = 0;
    static seq_t last_digit_sum = 0;

    /* initialise on first call */
    if (cached_terms == 0) {
        cache_size   = 16;
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0]     = 1;
        last_power   = 1;
        last_digit_sum = digit_sum(1);
        cached_terms = 1;
    }

    /* generate more terms until we have at least n of them */
    while (cached_terms < n) {
        /* next power of 4 */
        seq_t next = last_power * 4;

        /* guard against overflow in fixed‑width builds */
        if (next < last_power)            /* multiplication wrapped, overflow */
            break;

        seq_t next_sum = digit_sum(next);
        if (next_sum > last_digit_sum) {  /* strictly increasing digit sum */
            if (cached_terms >= cache_size) {
                cache_size *= 2;
                cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
            }
            cache[cached_terms] = next;
            cached_terms++;
            last_digit_sum = next_sum;
        }
        last_power = next;
    }

    /* return the requested term, or the last available if we ran out of range */
    if (n <= cached_terms)
        return cache[n - 1];
    return cache[cached_terms - 1];
}

/*
 * A166311  Numbers whose sum of digits is 11.
 * https://oeis.org/A166311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose sum of digits is 11 */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_candidate = 28;

seq_t a(idx_t n)
{
    /* Generate terms until we have n terms stored. */
    for (;;)
    {
        if (cache_len >= (size_t)n)
            break;

        ++last_candidate;                       /* next integer to test */
        if (digit_sum(last_candidate) == 11)    /* sum of decimal digits */
            append_candidate(&cache, &cache_len, &cache_cap, last_candidate);
    }

    return cache[n - 1];
}

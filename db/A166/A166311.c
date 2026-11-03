/*
 * A166311  Numbers whose sum of digits is 11.
 * https://oeis.org/A166311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 914 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose sum of digits is 11 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

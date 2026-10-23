/*
 * A031960  Numbers with exactly three distinct base-8 digits.
 * https://oeis.org/A031960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   append_candidate, has_three_distinct_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly three distinct base‑8 digits – computed
 * by brute‑force search with caching. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/has_three_distinct_base8.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 – behaviour below offset irrelevant */

    while (cache_len < n) {
        if (has_three_distinct_base8(last_checked)) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)last_checked);
        }
        ++last_checked;
    }

    return cache[n - 1];
}

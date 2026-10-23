/*
 * A043574  Numbers whose base-2 representation has exactly 7 runs.
 * https://oeis.org/A043574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_seven_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose binary representation has exactly 7 runs. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_seven_runs.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n terms in cache. */
    if (n > cache_size) {
        size_t need = (size_t)n - cache_size;
        while (cache_size < (size_t)n) {
            ++last_checked;
            if (has_seven_runs(last_checked)) {
                if (cache_size >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)last_checked;
            }
        }
    }
    return cache[n - 1];
}

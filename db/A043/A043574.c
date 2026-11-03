/*
 * A043574  Numbers whose base-2 representation has exactly 7 runs.
 * https://oeis.org/A043574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose binary representation has exactly 7 runs. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline int has_seven_runs(uint64_t x)
{
    /* Count runs in binary representation of x (x >= 1). */
    int runs = 1;                /* start with first run */
    int bit;                     /* current bit */
    int prev = 1;                /* leading bit is 1 */
    int pos = 63;                /* find position of highest set bit */
    while (((x >> pos) & 1) == 0)
        --pos;
    for (int i = pos - 1; i >= 0; --i) {
        bit = (int)((x >> i) & 1);
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
    }
    return runs == 7;
}

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
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)last_checked;
            }
        }
    }
    return cache[n - 1];
}

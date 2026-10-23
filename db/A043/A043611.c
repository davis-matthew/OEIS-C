/*
 * A043611  Numbers whose base-6 representation has exactly 3 runs.
 * https://oeis.org/A043611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑6 representation has exactly 3 runs
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   count_runs_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_runs_base6.h"

/* cache the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;
static idx_t  last_checked = 0;   /* the last integer examined */

/* returns the n‑th positive integer (n≥1) whose base‑6 representation has exactly 3 runs */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but harmless */

    /* compute further elements if necessary */
    if (seq_cache_len < (size_t)n) {
        for (idx_t ix = last_checked + 1; seq_cache_len < (size_t)n; ++ix) {
            seq_t val = (seq_t)ix;
            if (count_runs_base6(val) == 3) {
                /* grow the cache if needed */
                if (seq_cache_len + 1 > seq_cache_cap) {
                    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq_cache = tmp;
                    seq_cache_cap = new_cap;
                }
                seq_cache[seq_cache_len++] = val;
            }
            last_checked = ix;
        }
    }

    return seq_cache[n - 1];
}

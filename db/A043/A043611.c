/*
 * A043611  Numbers whose base-6 representation has exactly 3 runs.
 * https://oeis.org/A043611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑6 representation has exactly 3 runs
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* count the number of maximal runs in the base‑6 representation of x */
static int count_runs_base6(seq_t x)
{
    /* any positive x has at least one digit */
    int digits[64];
    int len = 0;
    seq_t tmp = x;

    while (tmp != 0) {
        digits[len++] = (int)(tmp % 6);
        tmp /= 6;
    }

    /* digits[] holds the number least significant digit first */
    int runs = 1;
    int prev = digits[len - 1];          /* most significant digit */
    for (int i = len - 2; i >= 0; --i) {
        int d = digits[i];
        if (d != prev) {
            ++runs;
            prev = d;
        }
    }
    return runs;
}

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
                    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
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

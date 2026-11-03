/*
 * A031960  Numbers with exactly three distinct base-8 digits.
 * https://oeis.org/A031960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly three distinct base‑8 digits – computed
 * by brute‑force search with caching. */
#include <stdlib.h>
#include <stdbool.h>
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

static bool has_three_distinct_base8(idx_t v)
{
    unsigned char mask = 0;
    if (v == 0) return false;             /* 0 has only the digit 0 */
    idx_t tmp = v;
    while (tmp) {
        unsigned int d = (unsigned int)(tmp & 7); /* v % 8 */
        mask |= 1U << d;
        tmp >>= 3;                        /* v /= 8 */
    }
    int cnt = 0;
    while (mask) {
        mask &= mask - 1;
        ++cnt;
        if (cnt > 3) return false;
    }
    return cnt == 3;
}

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

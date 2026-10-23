/*
 * A043595  Numbers whose base-4 representation has exactly 4 runs.
 * https://oeis.org/A043595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers whose base‑4 representation has exactly 4 runs. */
#include <stdlib.h>
#include <stdbool.h>

static inline bool has_four_runs(seq_t v)
{
    seq_t last = v % 4;          /* first base‑4 digit */
    int count = 1;               /* first run counted */
    v /= 4;
    while (v > 0) {
        seq_t d = v % 4;
        if (d != last) {
            ++count;
            if (count > 4) return false;  /* early exit */
        }
        last = d;
        v /= 4;
    }
    return count == 4;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0, len = 0;
    static seq_t cur = (seq_t)1;   /* current candidate, start from 1 */

    while (len < (size_t)n) {
        /* find next number with exactly 4 runs */
        while (!has_four_runs(cur)) ++cur;

        /* grow cache if needed */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        cache[len++] = cur;
        ++cur;   /* advance for future calls */
    }
    return cache[(size_t)n - 1];
}

/*
 * A045180  Numbers whose base-5 representation contains exactly two 0's and
 *          no 2's.
 * https://oeis.org/A045180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045180: Numbers whose base‑5 representation contains exactly two 0's and no 2's. */
/* The function generates the sequence in ascending order. */

#include <stdlib.h>
#include <stdbool.h>

/* Grow a dynamic array of seq_t values. */
static void grow_cache(seq_t **cache, size_t *len, size_t *cap) {
    if (*len == *cap) {
        size_t new_cap = (*cap == 0u) ? 16u : (*cap << 1u);
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* allocation failure */
        *cache = tmp;
        *cap   = new_cap;
    }
}

/* Recursively generate all length‑target_len base‑5 numbers that
   (i) have exactly two zeros,
   (ii) contain no digit 2,
   (iii) start with a non‑zero digit.  Numbers are produced
   in lexicographic (hence numeric) order. */
static void rec_generate(int pos, int target_len, int zero_cnt, seq_t value,
                         seq_t **cache, size_t *cnt, size_t *cap) {
    if (pos == target_len) {
        if (zero_cnt == 2) {
            grow_cache(cache, cnt, cap);
            (*cache)[*cnt] = value;
            (*cnt)++;
        }
        return;
    }
    if (zero_cnt > 2) return;   /* cannot have more than two zeros */

    if (pos == 0) {
        /* Leading digit: 1, 3, or 4 (no zero, no 2) */
        const int opts[3] = {1, 3, 4};
        for (int i = 0; i < 3; ++i) {
            int d = opts[i];
            rec_generate(pos + 1, target_len, zero_cnt,
                         value * (seq_t)5 + (seq_t)d,
                         cache, cnt, cap);
        }
    } else {
        /* Other positions: 0, 1, 3, or 4 */
        const int opts[4] = {0, 1, 3, 4};
        for (int i = 0; i < 4; ++i) {
            int d    = opts[i];
            int nz   = zero_cnt + (d == 0);
            rec_generate(pos + 1, target_len, nz,
                         value * (seq_t)5 + (seq_t)d,
                         cache, cnt, cap);
        }
    }
}

/* Main sequence function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of range */

    /* Static state: cache of generated terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static int    next_len  = 3;    /* next number length to generate */

    /* Generate terms until we have at least n of them */
    while ((size_t)n > cache_len) {
        rec_generate(0, next_len, 0, (seq_t)0,
                     &cache, &cache_len, &cache_cap);
        ++next_len;                  /* next batch will be longer numbers */
    }

    return cache[(size_t)n - 1];
}

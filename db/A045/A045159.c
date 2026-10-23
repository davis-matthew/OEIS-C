/*
 * A045159  Numbers whose base-5 representation contains no 0's and exactly
 *          two 1's.
 * https://oeis.org/A045159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   OEIS A045159 – numbers whose base‑5 representation contains no
 *            0's and exactly two 1's. The sequence is built by enumerating
 *            all base‑5 words of increasing length (digits 1–4) and selecting
 *            those with exactly two '1' digits
 * Range:     u8 n <= 37, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"

/* Append a value into a dynamically growing array. */
static inline void _append_value(seq_t **cache, size_t *len, size_t *cap,
                                 seq_t val)
{
    append_candidate(cache, len, cap, val);
}

/* Generate all words of length L (digits 1–4) with exactly two 1's
   and append their numeric values (base‑5) to the cache. */
static void _generate_len(idx_t L, seq_t **cache, size_t *len, size_t *cap)
{
    /* total number of words = 4^L, which fits in idx_t for L ≤ 64. */
    idx_t total = 1;
    for (idx_t i = 0; i < L; ++i)
        total *= 4;

    /* Temporary storage for digits of a word (most significant at [L‑1]). */
    unsigned int digits[64];   /* L is always < 64 for n ≤ 10000. */

    for (idx_t mask = 0; mask < total; ++mask) {
        idx_t t = mask;
        int ones = 0;

        /* Decode the word from least to most significant digit. */
        for (idx_t pos = 0; pos < L; ++pos) {
            unsigned int d4 = t % 4;  /* 0..3 */
            t /= 4;
            unsigned int d = d4 + 1;   /* 1..4 */
            digits[pos] = d;
            if (d == 1)
                ++ones;
        }

        if (ones != 2)
            continue;

        /* Convert the digit array to a base‑5 number. */
        seq_t val = 0;
        for (idx_t pos = 0; pos < L; ++pos) {
            /* Most significant digit first. */
            val = val * 5 + digits[L - 1 - pos];
        }

        _append_value(cache, len, cap, val);
    }
}

seq_t a(idx_t n)
{
    /* Sequence is 1‑based; values for n == 0 are undefined. */
    if (n == 0)
        return 0;

    /* Static cache of generated terms. */
    static seq_t *cache = NULL;
    static size_t len  = 0;
    static size_t cap  = 0;
    static idx_t next_len = 2;   /* generate words of length 2, 3, … */

    /* Grow the cache until we have at least 'n' terms. */
    while (len < n) {
        _generate_len(next_len, &cache, &len, &cap);
        ++next_len;
    }

    return cache[n - 1];
}

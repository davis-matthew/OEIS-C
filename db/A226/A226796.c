/*
 * A226796  Number of nonnegative numbers x < 10^n such that the digits of x^2
 *          occur with an equal frequency of 1.
 * https://oeis.org/A226796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of nonnegative x < 10^n such that the digits of x²
 *            are all distinct (each appears exactly once)
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_frequencies, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static state */
    static bool initialized = false;
    static seq_t *candidates = NULL;
    static idx_t cand_count = 0;
    static idx_t max_digit_len = 0;

    /* initialise once */
    if (!initialized) {
        const idx_t LIMIT_MAX = 1000000ULL;  /* 1e6 is more than enough for all qualifying numbers */
        candidates = (seq_t *)malloc(LIMIT_MAX * sizeof(seq_t));
        cand_count = 0;

        unsigned int freq[10];
        for (idx_t x = 0; x < LIMIT_MAX; ++x) {
            seq_t sq = (seq_t)x * (seq_t)x;
            digit_frequencies(sq, freq);
            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (freq[d] > 1) { ok = false; break; }
            }
            if (ok) {
                candidates[cand_count++] = (seq_t)x;
            }
        }

        /* shrink the array to the actual size */
        if (cand_count < LIMIT_MAX) {
            seq_t *tmp = (seq_t *)realloc(candidates, cand_count * sizeof(seq_t));
            if (tmp) candidates = tmp;
        }

        if (cand_count > 0)
            max_digit_len = digit_len(candidates[cand_count - 1]); /* number of decimal digits of the largest qualifying x */
        else
            max_digit_len = 0;

        initialized = true;
    }

    /* if 10ⁿ is larger than any qualifying number, all are counted */
    if (n >= max_digit_len)
        return (seq_t)cand_count;

    /* otherwise count those with x < 10ⁿ */
    seq_t limit = 1;
    for (idx_t i = 0; i < n; ++i) limit *= 10;

    idx_t count = 0;
    for (idx_t i = 0; i < cand_count; ++i) {
        if (candidates[i] < limit) ++count;
    }

    return (seq_t)count;
}

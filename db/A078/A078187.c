/*
 * A078187  Smaller of the two successive squares which differ in the use of
 *          only one digit.
 * https://oeis.org/A078187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smaller of each pair of successive squares m^2 and
 *            (m+1)^2 such that * the two squares differ in the use of exactly
 *            one digit
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   one_digit_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/one_digit_diff.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t stored = 0;
    static idx_t next_m = 1;

    if ((size_t)n > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cap = (size_t)n;
    }

    while (stored < n) {
        seq_t sq1 = (seq_t)next_m * (seq_t)next_m;
        seq_t sq2 = (seq_t)(next_m + 1) * (seq_t)(next_m + 1);
        if (one_digit_diff(sq1, sq2)) {
            cache[stored++] = sq1;
        }
        ++next_m;
    }
    return cache[n - 1];
}

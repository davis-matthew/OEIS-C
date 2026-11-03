/*
 * A044894  Base 8 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base-8 representation has
 *            runs of equal digits with lengths alternating even, odd, even, …
 *            (the first run must have even length)
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check whether a base-8 representation of x has alternating
   run lengths starting with even. */
static bool valid_base8_runs_even_odd(seq_t x)
{
    if (x == 0)
        return false;

    /* store digits (least significant first) */
    unsigned char digits[32];
    int len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 8);
        tmp /= 8;
    }

    bool expect_even = true;   /* first run must be even */
    int runlen = 1;

    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] == digits[i + 1]) {
            ++runlen;
        } else {
            if ((runlen & 1) != (expect_even ? 0 : 1))
                return false;
            expect_even = !expect_even;
            runlen = 1;
        }
    }
    /* final run */
    if ((runlen & 1) != (expect_even ? 0 : 1))
        return false;

    return true;
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t search_cur = 1;

    size_t target = (size_t)n;
    if (cache_len < target) {
        /* increase cache capacity if needed */
        if (cache_cap < target) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < target)
                new_cap *= 2;
            seq_t *newp = mem_resize(seq_t, cache, new_cap);
            cache = newp;
            cache_cap = new_cap;
        }

        /* extend cache up to required index */
        while (cache_len < target) {
            if (valid_base8_runs_even_odd(search_cur))
                cache[cache_len++] = search_cur;
            ++search_cur;
        }
    }

    return cache[n - 1];
}

/*
 * A065208  Numbers which need three 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer k such that the
 *            reverse‑and‑add process reaches a palindrome after exactly 3
 *            iterations, k itself not being a palindrome
 * Range:     u8 none, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"

static bool is_pal(seq_t x)
{
    return x == reverse_digits(x);
}

static bool needs_three_steps(seq_t k)
{
    seq_t cur = k;
    for (int step = 0; step < 3; ++step)
    {
        if (is_pal(cur))
            return false;          /* reached palindrome too early */
        cur += reverse_digits(cur);
    }
    return is_pal(cur);             /* must be palindrome after exactly 3 steps */
}

seq_t a(idx_t n)
{
    /* static cache of found values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_test = 1;

    /* grow cache if necessary */
    if (cache_len < n)
    {
        /* Ensure capacity */
        if (cache_cap == 0)
        {
            cache_cap = 16;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }
        while (cache_len < n)
        {
            /* Find next candidate */
            while (!needs_three_steps((seq_t)next_test))
            {
                ++next_test;
            }
            /* Store candidate */
            if (cache_len == cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_test;
            ++next_test;
        }
    }
    return cache[n - 1];
}

/*
 * A097271  Numbers that are neither the sum of two nonzero squares nor the
 *          difference of two nonzero squares.
 * https://oeis.org/A097271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th integer that is neither the sum of two nonzero
 *            squares nor the difference of two nonzero squares. The function
 *            generates terms by testing integers until the requested index is
 *            reached
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return true if n is not representable as a difference of two nonzero squares. */
static bool not_difference_of_two_nonzero_squares(seq_t n)
{
    /* 1 and 4 cannot be expressed as a^2 - b^2 with a,b>0. */
    if (n == 1 || n == 4)
        return true;
    /* Any integer congruent to 2 modulo 4 is also impossible. */
    if ((n % 4) == 2)
        return true;
    return false;
}

/* Return true if n can be expressed as a^2 + b^2 with a,b>0. */
static bool sum_of_two_nonzero_squares(seq_t n)
{
    if (n < 2)
        return false;                     /* 1 cannot be represented */
    /* largest a such that a^2 < n */
    seq_t limit_sq = floor_sqrt(n - 1);
    /* The loop counter is small enough to fit in idx_t */
    idx_t limit = (idx_t)limit_sq;
    for (idx_t a = 1; a <= limit; ++a) {
        seq_t a2 = (seq_t)a * a;
        seq_t r  = n - a2;
        if (r == 0)
            continue;                     /* would give b = 0 */
        seq_t b = floor_sqrt(r);
        if (b * b == r && b > 0)
            return true;                  /* found a representation */
    }
    return false;
}

/* Main sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of terms stored   */
    static size_t cache_cap  = 0;    /* allocated capacity       */
    static idx_t  next_case  = 1;    /* next integer to test    */

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        seq_t cand = (seq_t)next_case;

        /* Test the two impossible conditions */
        bool not_sum  = !sum_of_two_nonzero_squares(cand);
        bool not_diff =  not_difference_of_two_nonzero_squares(cand);

        if (not_sum && not_diff)
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    exit(EXIT_FAILURE);   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        ++next_case;
    }

    return cache[(size_t)(n - 1)];
}

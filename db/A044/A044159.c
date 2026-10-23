/*
 * A044159  Numbers n such that string 3,0 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth integer n such that its base‑7 representation contains
   the substring "30" but n−1 does not. */
#include <stdlib.h>
#include <stdbool.h>

/* return true iff x (in base 7) contains the substring 3 followed by 0.
   The digits are examined from least‑significant to most‑significant, so
   we look for d == 3 with the previously seen lower digit equal to 0. */
static bool has_30(seq_t x)
{
    bool seen = false;
    unsigned prev = 0;
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 7);
        if (seen && d == 3 && prev == 0)
            return true;
        prev = d;
        seen = true;
        x /= 7;
    }
    return false;
}

/* Return the n‑th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* stored sequence values   */
    static idx_t cache_size = 0;      /* number of values cached  */
    static idx_t cache_capacity = 0;  /* allocated capacity       */
    static seq_t next_to_check = 1;   /* candidate number         */

    while (cache_size < n)
    {
        /* find the next qualifying number */
        for (;;)
        {
            if (has_30(next_to_check) && !has_30(next_to_check - 1))
            {
                /* enlarge cache if necessary */
                if (cache_size == cache_capacity)
                {
                    cache_capacity = (cache_capacity == 0) ? 16 : cache_capacity * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   (size_t)cache_capacity * sizeof(seq_t));
                    if (!tmp) /* out of memory; fail safely */
                        return 0;
                    cache = tmp;
                }
                cache[cache_size++] = next_to_check;
                ++next_to_check;
                break;
            }
            ++next_to_check;
        }
    }
    return cache[n - 1];
}

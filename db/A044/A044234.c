/*
 * A044234  Numbers n such that string 5,7 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incrementally generate numbers whose base‑8 representation
   contains the substring “57” but the preceding number does not.
   The function memoises computed terms for efficiency. */
#include <stdlib.h>
#include <stdbool.h>

/* static cache for already computed terms */
static seq_t *seq_cache = NULL;
static idx_t seq_len = 0;      /* number of terms stored */
static idx_t seq_cap = 0;      /* allocated capacity */
static seq_t last_value = 0;   /* last term generated */

/* check whether the base‑8 representation of x contains the substring 57 */
static bool has57(seq_t x)
{
    int prev = -1;
    while (x > 0)
    {
        int cur = (int)(x % 8);   /* lower 3 bits are the current digit */
        if (prev == 7 && cur == 5)
            return true;          /* found 5 followed by 7 */
        prev = cur;
        x /= 8;
    }
    return false;
}

/* grow the cache to at least 'needed' elements */
static void grow_seq(idx_t needed)
{
    if (needed <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_arr;
    seq_cap = new_cap;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for non‑positive indexes */
        return 0;

    /* generate terms up to index n if required */
    if (seq_len < n)
    {
        grow_seq(n);
        for (idx_t i = seq_len; i < n; ++i)
        {
            seq_t candidate = last_value + 1;
            while (1)
            {
                if (has57(candidate) && !has57(candidate - 1))
                {
                    seq_cache[i] = candidate;
                    last_value = candidate;
                    break;
                }
                ++candidate;
            }
        }
        seq_len = n;
    }
    return seq_cache[n - 1];
}

/*
 * A044090  Numbers n such that string 2,3 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044090:
   Numbers n such that the string "23" occurs in the base‑5 representation of n
   but not in that of n‑1.  Implements the sequence by a linear search and
   memoises results so that arbitrary order of calls is supported. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: true if base‑5 representation of x contains the digit pair 2,3
   (most significant → least significant). */
static bool has23(seq_t x)
{
    /* Store up to 64 base‑5 digits; more than enough for 128‑bit numbers. */
    uint8_t digits[64];
    size_t len = 0;
    const seq_t FIVE = (seq_t)5;

    /* Special case: x == 0 has no digits. */
    if (x == (seq_t)0)
        return false;

    while (x > (seq_t)0)
    {
        digits[len++] = (uint8_t)(x % FIVE);
        x /= FIVE;
    }
    /* Look for adjacent digits 3 (less significant) followed by 2. */
    for (size_t i = 0; i + 1 < len; ++i)
        if (digits[i] == 3 && digits[i + 1] == 2)
            return true;
    return false;
}

/* Cache variables */
static seq_t *cached_seq = NULL;   /* array of found numbers */
static size_t cached_len = 0;      /* number of cached terms */
static size_t cached_cap = 0;      /* allocated capacity */
static seq_t next_candidate = 1;   /* next integer to test */

/* Grow the cache array if needed. */
static void grow_cache(void)
{
    if (cached_cap == 0)
        cached_cap = 64;
    else
        cached_cap <<= 1;
    cached_seq = (seq_t *)realloc(cached_seq, cached_cap * sizeof(seq_t));
}

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Validate: n >= 1 per offset. */
    if (n == (idx_t)0)
        return (seq_t)0;  /* unspecified, but return 0 */

    /* Ensure the cache contains at least n terms. */
    while (cached_len < (size_t)n)
    {
        seq_t cur = next_candidate;
        next_candidate = cur + (seq_t)1;      /* advance candidate */

        if (has23(cur) && !has23(cur - (seq_t)1))
        {
            if (cached_len == cached_cap)
                grow_cache();
            cached_seq[cached_len++] = cur;
        }
    }

    return cached_seq[(size_t)n - 1];
}

/*
 * A098729  Consider the sequence {b(n), n >= 1} of digits of the integers: 0
 *          1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9 2 0...
 *          (A033307); a(n) = b(n) + n.
 * https://oeis.org/A098729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = digit_at_position(n) + n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit at position n is taken from the concatenation of all
   decimal digits of the integers 0,1,2,… starting from 0.  The
   sequence a(n) is obtained by adding the index n to that digit. */
#include <stdint.h>   /* for uint64_t, etc. */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1, but handle n==0 defensively. */
    if (n == 0)
        return 0;

    /* position in the digit stream (1‑based) */
    idx_t pos = n;

    /* variables used while scanning numbers by decimal length */
    idx_t len = 1;          /* current decimal length */
    idx_t first = 0;        /* first number having current length */
    idx_t pow10prev = 1;    /* 10^(len-1) */
    unsigned __int128 cumulative = 0;  /* cumulative digits processed */

    while (1)
    {
        /* How many numbers have this length? */
        idx_t countLen = (len == 1) ? 10 : 9 * pow10prev;

        /* Total digits contributed by this block of numbers. */
        unsigned __int128 seg_digits  = (unsigned __int128)countLen * len;

        /* Check whether the desired position lies within this block. */
        if (cumulative + seg_digits >= (unsigned __int128)pos)
        {
            /* Offset inside the block (0‑based). */
            idx_t offset = (idx_t)(pos - cumulative - 1U);

            /* Which number (0‑based) in this block? */
            idx_t numberIndex = offset / len;

            /* Which digit (0‑based from most significant). */
            idx_t digitIndex = offset % len;

            /* The actual number: first + numberIndex. */
            idx_t number = first + numberIndex;

            /* Divisor to isolate the requested digit. */
            idx_t divisor = 1;
            for (idx_t i = 0; i < (len - 1 - digitIndex); ++i)
                divisor *= 10U;

            /* Extracted digit. */
            seq_t digit = (seq_t)((number / divisor) % 10U);

            /* Return the sequence value: n + digit. */
            return (seq_t)n + digit;
        }

        /* Prepare for the next length block. */
        cumulative += seg_digits;
        first      += countLen;
        pow10prev  *= 10U;
        ++len;
    }
}

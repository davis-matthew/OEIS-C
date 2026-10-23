/*
 * A039576  Numbers whose base-6 representation has the same number of 0's,
 *          1's and 5's.
 * https://oeis.org/A039576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A039576
 *
 * A039576 = { n ≥ 1 | in the base‑6 representation of n the
 *             numbers of digits 0, 1 and 5 are equal }.
 *
 * The function builds the sequence lazily, caching previously found
 * terms.  It works for any of the supported seq_t types
 * (including arbitrary‑precision big integers) and for any size_t
 * of idx_t (uint64_t).  It uses only the allowed operators and
 * standard library functions.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cache of already found terms                       */
    static seq_t *seq = NULL;
    static size_t len = 0, cap = 0;
    static idx_t next_candidate = 2;    /* first positive integer to test */

    /* Grow cache until we have at least n terms           */
    while (len < (size_t)n)
    {
        idx_t x = next_candidate++;      /* next candidate to test   */
        idx_t t = x;
        unsigned int cnt0 = 0, cnt1 = 0, cnt5 = 0;

        /* Count the digits 0,1,5 in base‑6 representation of x */
        while (t > 0)
        {
            int d = t % 6;
            if (d == 0)       cnt0++;
            else if (d == 1)  cnt1++;
            else if (d == 5)  cnt5++;
            t /= 6;
        }

        /* Keep term only if the three counts are equal     */
        if (cnt0 == cnt1 && cnt1 == cnt5)
        {
            /* Ensure capacity of the array                  */
            if (len == cap)
            {
                size_t new_cap = (cap == 0 ? 16 : cap * 2);
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp)   /* out of memory: let the program abort   */
                    abort();
                seq = tmp;
                cap = new_cap;
            }
            seq[len++] = (seq_t)x;
        }
    }
    return seq[(size_t)n - 1];
}

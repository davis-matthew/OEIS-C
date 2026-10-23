/*
 * A044582  Numbers n such that string 1,6 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates sequence A044582:
   numbers n whose base‑8 representation contains the substring "16"
   but the base‑8 representation of n+1 does not. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑8 digits of x contain the adjacent pair 1,6. */
static bool has16(seq_t x)
{
    if (x == (seq_t)0)
        return false;                       /* zero has no digits 1 and 6 */

    seq_t tmp = x;
    uint8_t prev = 0;                       /* previous digit (more significant) */

    while (tmp > (seq_t)0)
    {
        uint8_t d = (uint8_t)(tmp % (seq_t)8);
        tmp   /= (seq_t)8;

        /* In the reversed digit stream: d is the current less‑significant digit
           and prev is the next more significant digit.  We found the pattern
           1 (more significant) followed by 6 (less significant). */
        if (d == 1 && prev == 6)
            return true;

        prev = d;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached sequence values */
    static size_t len   = 0;         /* number of values cached */
    static size_t cap   = 0;         /* allocated capacity of cache */
    static idx_t cur    = 0;         /* next integer to test (as idx_t) */

    if (n == 0)
        return (seq_t)0;              /* undefined for 0; not used */

    /* Generate terms until the desired index is cached. */
    while (len < (size_t)n)
    {
        seq_t cur_seq = (seq_t)cur;

        if (has16(cur_seq))
        {
            seq_t next_seq = cur_seq + (seq_t)1;
            if (!has16(next_seq))
            {
                /* Grow cache if necessary */
                if (len == cap)
                {
                    size_t new_cap = cap ? (cap << 1) : 128;
                    seq_t *new_cache = (seq_t *)realloc(cache,
                                                        new_cap * sizeof(seq_t));
                    if (!new_cache)
                        exit(1);              /* abort on allocation failure */
                    cache = new_cache;
                    cap   = new_cap;
                }
                cache[len++] = cur_seq;
            }
        }
        cur++;                           /* move to next integer */
    }

    return cache[n - 1];
}

/*
 * A151740  Composites that are the sum of two consecutive composite numbers.
 * https://oeis.org/A151740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates A151740.
   It scans the list of composite integers and records every
   composite sum of two adjacent composites.  The sequence is
   constructed lazily and memoised so that calls with arbitrary
   indices are answered in O(1) time after the required prefix
   has been computed. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, guard against underflow */

    /* Memoisation table for the sequence */
    static seq_t *seq = NULL;
    static size_t len = 0;        /* number of values stored */
    static size_t cap = 0;        /* allocated capacity */

    /* State of the combinatorial scan */
    static seq_t last_comp = 0;
    static bool last_set   = false;
    static seq_t next_int  = 4;   /* smallest composite to examine */

    /* Grow the table until it contains n terms */
    while (len < (size_t)n)
    {
        /* Find the next composite number */
        seq_t cand = next_int;
        while (cand < 4 || is_prime_small(cand))
            cand = cand + 1;

        /* If we have a preceding composite, test the sum */
        if (last_set)
        {
            seq_t sum = last_comp + cand;
            if (sum >= 4 && !is_prime_small(sum))
            {
                /* Ensure enough capacity */
                if (len == cap)
                {
                    size_t old_cap = cap;
                    cap = cap ? cap * 2 : 32;
                    seq = seq_realloc(seq, old_cap, cap);
                    if (!seq) exit(1);                /* fatal allocation failure */
                }
                seq[len++] = sum;
            }
        }

        /* Advance the state for the next scan step */
        last_comp = cand;
        last_set  = true;
        next_int  = cand + 1;
    }

    return seq[(size_t)n - 1];
}

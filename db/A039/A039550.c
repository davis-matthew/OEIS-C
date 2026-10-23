/*
 * A039550  Numbers whose base-12 representation has the same nonzero number
 *          of 7's and 9's.
 * https://oeis.org/A039550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A039550: Numbers whose base-12 representation has the same nonzero
 *          number of 7's and 9's.
 *
 * The routine finds the terms by brute‑force enumeration, keeping a
 * cache of already-found values.  It grows the cache dynamically.
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of terms found so far */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;   /* array capacity */
    static size_t terms_cnt = 0;   /* number of terms stored */
    static seq_t next_candidate = 1; /* next integer to test */

    /* make sure the cache holds at least n terms */
    while (terms_cnt < (size_t)n)
    {
        /* count 7's and 9's in the base‑12 representation of next_candidate */
        seq_t m = next_candidate;
        int c7 = 0, c9 = 0;
        while (m != 0)
        {
            int digit = (int)(m % 12);
            if (digit == 7) ++c7;
            else if (digit == 9) ++c9;
            m = m / 12;
        }
        if (c7 > 0 && c7 == c9)
        {
            /* store term */
            if (terms_cnt == terms_cap)
            {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}

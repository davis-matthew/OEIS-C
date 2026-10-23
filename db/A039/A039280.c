/*
 * A039280  Numbers whose base-4 representation has the same nonzero number of
 *          1's and 2's.
 * https://oeis.org/A039280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer whose base‑4 representation has the
 *            same nonzero number of 1's as 2's
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *terms = NULL;
    static idx_t terms_cap = 0;
    static idx_t terms_cnt = 0;
    static seq_t next_val = 1; /* next integer to examine */

    while (terms_cnt < n)
    {
        /* Find next term satisfying the property. */
        while (1)
        {
            seq_t v = next_val++;
            int c1 = 0, c2 = 0;
            seq_t x = v;

            while (x)
            {
                int d = (int)(x & 3);
                if (d == 1) ++c1;
                else if (d == 2) ++c2;
                x >>= 2;
            }
            if (c1 == c2 && c1 > 0)
            {
                /* Grow cache if necessary. */
                if (terms_cnt == terms_cap)
                {
                    idx_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* safe‑guard */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_cnt++] = v;
                break;
            }
        }
    }

    return terms[n - 1];
}

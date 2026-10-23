/*
 * A043511  Numbers having three 5's in base 10.
 * https://oeis.org/A043511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer having exactly three digit '5's in decimal
 * Range:     u8 none, u16 n <= 680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    static seq_t *terms = NULL;
    static idx_t terms_len = 0;
    static idx_t cap = 0;
    static seq_t next_candidate = 555; /* first candidate */

    /* initial allocation */
    if (terms_len == 0)
    {
        cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms)
            return 0; /* allocation failed */
    }

    /* generate terms until we have at least n */
    while (terms_len < n)
    {
        seq_t x = next_candidate;

        /* count occurrences of digit 5 */
        idx_t cnt = 0;
        seq_t temp = x;
        while (temp > 0)
        {
            if ((temp % 10) == 5)
                ++cnt;
            temp = temp / 10;
        }

        if (cnt == 3)
        {
            if (terms_len == cap)
            {
                cap <<= 1;
                seq_t *new_terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!new_terms)
                    return 0; /* allocation failed */
                terms = new_terms;
            }
            terms[terms_len++] = x;
        }

        ++next_candidate;
    }

    return terms[n - 1];
}

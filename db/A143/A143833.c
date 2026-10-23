/*
 * A143833  Numbers n such that 14n^2 - 1 is prime.
 * https://oeis.org/A143833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 14*n*n - 1 is prime */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t tested_upto = 0;   /* last n tested for 14*n^2-1 */

    while (terms_len < (size_t)n)
    {
        ++tested_upto;

        /* compute 14 * tested_upto^2 - 1 in seq_t */
        seq_t m = (seq_t)tested_upto;
        seq_t t = m * m;     /* tested_upto^2 */
        t = t * 14;          /* 14 * tested_upto^2 */
        t = t - 1;           /* 14 * tested_upto^2 - 1 */

        if (is_prime_big(t))
        {
            if (terms_len >= terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = m;
        }
    }

    return terms[(size_t)n - 1];
}

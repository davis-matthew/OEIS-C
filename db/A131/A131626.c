/*
 * A131626  Prime subsequences in the evaluation of the first derivatives of
 *          quadratic equations with positive successive prime coefficients.
 * https://oeis.org/A131626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131626: concatenate primes of the form 2*prime(p)*x + prime(p+1)
   for successive integers x, stopping each row when the expression
   first becomes composite.  The sequence a(1), a(2), … is the list
   of these primes in order of generation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset */

    /* cache of generated terms */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;

    /* current row and prime index */
    static idx_t cur_x = 1;               /* current x  */
    static idx_t cur_p = 1;               /* current p  */

    while ((size_t)len < (size_t)n)
    {
        /* compute 2*prime(p)*x + prime(p+1) as a seq_t expression */
        seq_t value = (seq_t)2 * get_prime(cur_p) * (seq_t)cur_x
                      + get_prime(cur_p + 1);

        if (is_prime_big(value))
        {
            /* store the prime in the cache */
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;      /* allocation failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = value;
            cur_p++;                     /* advance to next p in this row */
        }
        else
        {
            /* row finished at this p: move to next x */
            cur_x++;
            cur_p = 1;
        }
    }

    return cache[n - 1];
}

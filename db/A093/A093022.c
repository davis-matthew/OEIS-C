/*
 * A093022  Indices of terms in A093019 with value 2.
 * https://oeis.org/A093022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th integer whose Luhn check digit is 2.
   The sequence is defined via brute‑force search: for each positive
   integer m, the Luhn check digit is computed; if it equals 2,
   m is appended to the list.  a(1)=4, a(2)=18, ...       */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache holding the found indices.  The cache is grown
       lazily as more terms are requested. */
    static seq_t *cache = NULL;          /* array of found terms   */
    static size_t cache_size = 0;        /* number of terms stored */
    static size_t cache_cap = 0;         /* allocated capacity     */
    static idx_t checked = 0;            /* last integer examined  */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        ++checked;                       /* examine next integer   */

        /* Compute the Luhn check digit for 'checked'.
           Double every other digit starting from the least
           significant (i.e., the rightmost digit gets doubled
           because it will be in position 2 once the check digit
           is appended). */
        seq_t sum = 0;
        int double_flag = 1;                /* 1 ⇒ next digit doubled   */
        idx_t tmp = checked;
        while (tmp > 0)
        {
            int digit = tmp % 10;
            tmp /= 10;
            if (double_flag)
            {
                int dbl = digit * 2;
                if (dbl > 9) dbl -= 9;
                sum += (seq_t)dbl;
            }
            else
            {
                sum += (seq_t)digit;
            }
            double_flag ^= 1;               /* toggle the flag          */
        }

        int check_digit = (10 - (int)(sum % 10)) % 10;

        /* If the required check digit is 2, store the integer. */
        if (check_digit == 2)
        {
            if (cache_size == cache_cap)    /* grow the array if full    */
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) /* allocation failure: abort silently */
                    return 0;
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = checked;
        }
    }

    /* Return the requested term.  The sequence is 1‑based. */
    return cache[(size_t)n - 1];
}

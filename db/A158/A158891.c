/*
 * A158891  Numbers n with property that (2/(n+1))*(Concatenated triangular
 *          numbers T (n), T(n+1)) is prime.
 * https://oeis.org/A158891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth integer k such that (2/(k+1)) *
 *            concat(T(k),T(k+1)) is prime, where T(k)=k*(k+1)/2. The sequence
 *            is 1-indexed
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static idx_t candidate = 1;          /* next integer to test */

    /* Expand the cache until we have at least n elements */
    while (sz < n) {
        /* Triangular numbers */
        seq_t tn = (seq_t)candidate * ((seq_t)candidate + 1) / 2;
        seq_t tn1 = (seq_t)(candidate + 1) *
                     ((seq_t)(candidate + 1) + 1) / 2;

        /* Concatenate the two triangular numbers */
        seq_t concat = concat_parts((idx_t)tn, (idx_t)tn1);

        /* Compute (2*concat)/(candidate+1) */
        seq_t product = 2 * concat;
        seq_t den = (seq_t)(candidate + 1);

        seq_t q = product / den;
        seq_t check = q * den;

        if (check == product && is_prime_big(q)) {
            /* Store the candidate */
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[sz++] = (seq_t)candidate;
        }

        candidate++;
    }

    return cache[n - 1];
}

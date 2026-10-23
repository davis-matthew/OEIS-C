/*
 * A152769  Numbers n such that pi(n) is nonprime.
 * https://oeis.org/A152769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   free_seq_array, is_prime, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers n such that π(n) is nonprime (pi(n) is nonprime). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* cache of computed terms */
    static seq_t *terms    = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    /* state kept between calls */
    static idx_t next_n = 1;      /* next integer to examine          */
    static idx_t pi_cnt = 0;      /* π(next_n-1)                       */

    /* ensure the cache contains at least n terms */
    if (terms_len < n) {
        /* grow cache capacity if needed */
        size_t need = n;
        if (need > terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            while (new_cap < need) new_cap *= 2;

            seq_t *tmp = seq_alloc_new_array(new_cap);
            if (terms) {
                for (size_t i = 0; i < terms_len; ++i)
                    tmp[i] = terms[i];
                free_seq_array(terms);
            }
            terms     = tmp;
            terms_cap = new_cap;
        }

        /* generate terms until we have n of them */
        while (terms_len < n) {
            idx_t x = next_n;
            if (is_prime(x)) ++pi_cnt;

            bool nonprime = (pi_cnt <= 1) || !is_prime(pi_cnt);
            if (nonprime)
                terms[terms_len++] = (seq_t)x;

            ++next_n;
        }
    }

    return terms[n - 1];
}

/*
 * A229221  Numbers k such that k - (product of digits of k) is prime.
 * https://oeis.org/A229221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number k such that k - product_of_digits(k) is prime */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache of previously found terms */
    static seq_t *terms = NULL;
    static size_t term_cnt = 0;
    static size_t capacity = 0;

    if (n == 0) return 0; /* guard: sequence starts at offset 1 */

    /* Search until we have n terms cached */
    while (term_cnt < n)
    {
        /* start searching from one more than the last cached term */
        seq_t k = (term_cnt == 0) ? 1 : (terms[term_cnt - 1] + 1);

        for (;; ++k)
        {
            seq_t prod = digit_product(k);       /* product of digits of k */
            if (k < prod) continue;              /* avoid negative difference */
            seq_t diff = k - prod;
            if (diff < 2) continue;              /* prime must be >= 2 */
            if (is_prime_big(diff))
            {
                /* Ensure enough space to store the new term */
                if (term_cnt == capacity)
                    ensure_capacity(term_cnt + 1, &capacity, (void **)&terms, sizeof(seq_t));
                terms[term_cnt++] = k;
                break; /* proceed to find the next term */
            }
        }
    }

    return terms[n - 1];
}

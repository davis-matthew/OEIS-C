/*
 * A101299  Numbers n such that -1 + Sum_{x=1..n} phi(x) is a prime number.
 * https://oeis.org/A101299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer m such that −1 + Σ_{x=1..m} φ(x) is
 *            prime
 * Range:     u8 n <= 18, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"  /* provides bool is_prime_seq(seq_t) */

/* The function returns the n-th term of OEIS A101299. */
seq_t a(idx_t n)
{
    /* Static state shared by all calls */
    static idx_t   current_limit  = 0;      /* largest m processed so far */
    static seq_t   cumulative_sum = 0;      /* Σ_{x=1..current_limit} φ(x)   */
    static seq_t *terms          = NULL;    /* collected values of m     */
    static idx_t  terms_count    = 0;       /* number of terms stored    */
    static idx_t  terms_capacity = 0;       /* allocated space for terms */

    /* Ensure we have at least n terms computed */
    while (terms_count < n) {
        /* Pick a new upper bound to extend across */
        idx_t new_limit = (current_limit ? current_limit * 2 : 100);
        if (new_limit <= current_limit) new_limit = current_limit + 1;

        /* Allocate an array for φ values up to new_limit */
        idx_t *phi = (idx_t *)malloc((new_limit + 1) * sizeof(idx_t));
        if (!phi) exit(1);

        /* Initialize φ[i] = i */
        for (idx_t i = 0; i <= new_limit; ++i) phi[i] = i;
        phi[0] = 0;
        if (new_limit >= 1) phi[1] = 1;

        /* Compute φ in O(n log log n) using a simple sieve */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (phi[i] == i) {                    /* i is prime */
                for (idx_t j = i; j <= new_limit; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }

        /* Update cumulative sum and collect new qualifying values */
        for (idx_t i = current_limit + 1; i <= new_limit; ++i) {
            cumulative_sum += (seq_t)phi[i];
            if (is_prime_seq(cumulative_sum - 1)) {
                if (terms_count == terms_capacity) {
                    terms_capacity = terms_capacity ? terms_capacity * 2 : 8;
                    terms = (seq_t *)realloc(terms,
                        terms_capacity * sizeof(seq_t));
                    if (!terms) exit(1);
                }
                terms[terms_count++] = (seq_t)i;
            }
        }

        /* Release temporary storage and update the processed limit */
        free(phi);
        current_limit = new_limit;
    }

    /* Return the requested term */
    return terms[n - 1];
}

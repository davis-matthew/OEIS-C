/*
 * A050754  Seventh powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth seventh power (k^7) that contains no pair * of
 *            consecutive equal decimal digits. The sequence is built on
 *            demand * and cached for arbitrary order of queries
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   no_consecutive_equal_digits, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/pow_uint.h"
#include "../utils/no_consecutive_equal_digits.h"

seq_t a(idx_t n) {
    /* Static cache of already computed terms */
    static seq_t *terms      = NULL;
    static idx_t term_count  = 0;   /* number of terms in cache */
    static idx_t term_cap    = 0;   /* capacity of cache */
    static idx_t next_k      = 0;   /* next integer to test */

    /* Ensure we have at least n+1 terms */
    while (term_count <= n) {
        seq_t val = pow_uint(next_k, 7);          /* compute k^7 */
        if (no_consecutive_equal_digits(val)) {
            /* Grow cache if necessary */
            if (term_count == term_cap) {
                idx_t new_cap = term_cap == 0 ? 1 : term_cap << 1;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failed */
                if (terms) memcpy(tmp, terms,
                                 term_count * sizeof(seq_t));
                free(terms);
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_count++] = val;
        }
        next_k++;
    }
    return terms[n];
}

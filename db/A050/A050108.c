/*
 * A050108  a(n) = floor(a(n-1)/4) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 5*a(n-1).
 * https://oeis.org/A050108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; For n>1, a(n)=floor(a(n-1)/4) if that value is positive
 *            and not yet seen, otherwise a(n)=5*a(n-1)
 * Range:     u8 n <= 19, u16 n <= 213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute or fetch the n-th term of A050108. */
seq_t a(idx_t n)
{
    /* Maximum number of terms we currently store. */
    static size_t cache_size = 0;
    /* Number of terms currently cached (equals the highest computed index). */
    static idx_t term_count = 0;
    /* Dynamic array holding the sequence values. */
    static seq_t *terms = NULL;

    /* Ensure the array can hold the requested term. */
    if ((idx_t)n > term_count) {
        /* Extend cache size if necessary. */
        size_t needed = (size_t)n;
        if (cache_size < needed) {
            size_t new_size = cache_size == 0 ? 64 : cache_size * 2;
            if (new_size < needed) new_size = needed;
            seq_t *tmp = (seq_t *)realloc(terms, new_size * sizeof(seq_t));
            if (!tmp) {
                /* Out of memory – aborting. */
                abort();
            }
            terms = tmp;
            cache_size = new_size;
        }

        /* Initialize missing elements. */
        /* first term */
        if (term_count == 0) {
            terms[0] = 1;
            term_count = 1;
        }

        /* Compute subsequent terms up to n. */
        for (idx_t i = term_count + 1; i <= n; ++i) {
            seq_t prev = terms[i - 2];   /* a(i-1) */
            seq_t cand = prev / 4;       /* floor division by 4 */

            bool seen = false;
            if (cand > 0) {
                for (idx_t j = 0; j < i - 1; ++j) {
                    if (terms[j] == cand) {
                        seen = true;
                        break;
                    }
                }
            }

            seq_t next;
            if (cand > 0 && !seen) {
                next = cand;
            } else {
                next = 5 * prev;
            }

            terms[i - 1] = next;
        }
        term_count = n;
    }

    return terms[n - 1];
}

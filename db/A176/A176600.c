/*
 * A176600  Numbers n such that concatenations n//13 and n//31 are consecutive
 *          primes.
 * https://oeis.org/A176600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 12, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: n such that 100*n+13 and 100*n+31 are consecutive primes. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache for the generated terms */
    static seq_t *terms = NULL;          /* array of terms */
    static size_t terms_cap = 0;         /* allocated capacity */
    static size_t terms_len = 0;         /* number of terms stored */
    static idx_t last_checked = 0;       /* last n examined */

    if (n == 0) return 0; /* undefined for index 0 */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Candidate n to test */
        idx_t cand = last_checked + 1;

        /* Construct the two concatenations: 100*n+13 and 100*n+31 */
        seq_t p = (seq_t)cand * 100 + 13;
        seq_t q = (seq_t)cand * 100 + 31;

        /* Test primality of both numbers */
        if (is_prime_big(p) && is_prime_big(q)) {
            /* Ensure that there is no other prime between p and q
               (they must be consecutive primes). The difference is 18,
               so we only need to check the seven odd numbers between */
            bool consecutive = true;
            for (seq_t x = p + 2; x < q; x += 2) {
                if (is_prime_big(x)) {
                    consecutive = false;
                    break;
                }
            }
            if (consecutive) {
                /* Expand the cache if necessary */
                if (terms_len + 1 > terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                    seq_t *new_terms = NULL;

#ifdef __cplusplus
                    new_terms = new seq_t[new_cap];
                    for (size_t i = 0; i < terms_len; ++i)
                        new_terms[i] = terms[i];
                    if (terms) delete[] terms;
#else
                    new_terms = realloc(terms, new_cap * sizeof(seq_t));
#endif
                    if (!new_terms) abort(); /* allocation failure */
                    terms = new_terms;
                    terms_cap = new_cap;
                }
                /* Store the found n */
                terms[terms_len++] = (seq_t)cand;
            }
        }

        last_checked = cand;
    }

    return terms[n - 1];
}

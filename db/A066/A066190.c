/*
 * A066190  Numbers k such that the sum of the even aliquot parts of k divides
 *          k.
 * https://oeis.org/A066190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers k such that the sum of the even proper divisors
 *            of k divides k. * Computation: sieve the sums of even proper
 *            divisors up to a growing limit * and collect the qualifying
 *            numbers until at least the desired amount has been found. * The
 *            implementation uses static storage so successive...
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */
#include "../utils/build_terms.h"

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;
    static idx_t  terms_len = 0;
    static idx_t  limit     = 0;

    if (n == 0) return 0;                 /* undefined for offset 1 */

    /* Grow the term list until we have at least n entries */
    if (terms_len < n) {
        seq_t *new_terms = build_terms(n, &terms_len, &limit);
        /* free previous storage if we had a previous allocation */
        free(terms);
        terms = new_terms;
    }

    return terms[n - 1];                  /* a(1) corresponds to terms[0] */
}

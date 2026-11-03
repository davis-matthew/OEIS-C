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
 * Validated: standard, pass, 72 terms through n = 962 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */
#include "../utils/mem.h"

static seq_t *build_terms(idx_t target_n, idx_t *terms_len, idx_t *limit_ptr)
{
    static idx_t current_limit = 0;                 /* last limit used */
    idx_t limit = (current_limit > 0 ? current_limit : (idx_t)200);
    seq_t *terms = NULL;                           /* array of qualifying numbers */
    size_t len   = 0;                               /* elements stored so far */
    size_t cap   = 0;                               /* allocated capacity */

    /* Grow until we have at least target_n terms */
    while (len < target_n) {
        limit <<= 1;                                 /* double the limit */

        /* Allocate array of sums of even proper divisors (initialized to 0) */
        seq_t *sum_even = mem_new(seq_t, (size_t)(limit + 1));
        if (!sum_even) return NULL;                 /* allocation failure */

        /* Sieve: add each even divisor to all its multiples > divisor */
        for (idx_t d = 2; d <= limit / 2; d += 2) {
            seq_t sd = (seq_t)d;                     /* cast to seq_t */
            for (idx_t m = d << 1; m <= limit; m += d)
                sum_even[m] += sd;
        }

        /* Collect qualifying numbers */
        for (idx_t i = 2; i <= limit; ++i) {
            seq_t s = sum_even[i];
            if (s > 0 && ((seq_t)i % s) == 0) {
                if (len == cap) {
                    cap = (cap ? cap << 1 : 16);
                    seq_t *tmp = mem_resize(seq_t, terms, cap);
                    if (!tmp) { mem_free(sum_even); mem_free(terms); return NULL; }
                    terms = tmp;
                }
                terms[len++] = (seq_t)i;
            }
        }
        mem_free(sum_even);                         /* sums no longer needed */

        /* If still insufficient, loop again with a larger limit */
    }

    current_limit = limit;
    *terms_len = (idx_t)len;
    *limit_ptr = limit;
    return terms;                                 /* caller must free when done */
}

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
        mem_free(terms);
        terms = new_terms;
    }

    return terms[n - 1];                  /* a(1) corresponds to terms[0] */
}

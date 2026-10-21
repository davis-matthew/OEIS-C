/*
 * A076483  a(n) = n!*Sum_{k=1..n} (k-1)^k/k!.
 * https://oeis.org/A076483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=1..n} (k-1)^k / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(n+1) = (n+1)*a(n) + n^{n+1} */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_size = 0;

    /* Allocate the cache if necessary */
    if (terms == NULL) {
        terms_size = (size_t)1;
        terms = (seq_t *)malloc(terms_size * sizeof(seq_t));
        if (!terms) return (seq_t)0;          /* allocation failed – safe return */
        terms[0] = (seq_t)0;
    }

    /* Extend the cache to include n */
    if ((size_t)n >= terms_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed – safe return */
        terms = tmp;
        for (idx_t i = terms_size; i <= n; ++i) {
            /* Compute i^{i-1} ^ i = (i-1)^i */
            seq_t pow = (seq_t)1;
            seq_t base = (seq_t)(i - 1);
            for (idx_t j = 0; j < i; ++j) {
                pow *= base;                /* unsigned overflow is defined modulo 2^N */
            }
            terms[i] = ((seq_t)i) * terms[i - 1] + pow;
        }
        terms_size = new_size;
    }

    return terms[(size_t)n];
}

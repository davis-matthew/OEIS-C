/*
 * A052605  Expansion of E.g.f. x*(1-x)/(1-x-x^3).
 * https://oeis.org/A052605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=0, a(3)=0.
   For n≥0:  a(n+3) = (n^3+6n^2+11n+6)*a(n) + (n+3)*a(n+2). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *seq  = NULL;
    static size_t len  = 0;
    static size_t cap  = 0;

    /* Initialise base values once */
    if (len == 0) {
        cap = 8;                            /* start with a little more than needed */
        seq = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!seq) abort();
        seq[0] = 0;
        seq[1] = 1;
        seq[2] = 0;
        seq[3] = 0;
        len   = 4;
    }

    if ((size_t)n < len) return seq[(size_t)n];

    /* If we need more terms, grow the cache */
    if (len <= (size_t)n) {
        /* Double capacity until it is larger than the index needed */
        while (cap <= (size_t)n) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) { free(seq); abort(); }
            seq = tmp;
            cap = new_cap;
        }

        /* Compute terms from the first missing index up to n */
        for (size_t i = len; i <= (size_t)n; ++i) {
            size_t k = i;                    /* k = i  (the term we compute) */
            seq_t b = (seq_t)(k - 3);        /* n = k-3 in the recurrence */
            seq_t coeff = b*b*b + 6*b*b + 11*b + 6;
            seq[k] = coeff * seq[k - 3] + (seq_t)k * seq[k - 1];
        }
        len = (size_t)n + 1;
    }

    return seq[(size_t)n];
}

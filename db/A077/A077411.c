/*
 * A077411  Combined Diophantine Chebyshev sequences A077409 and A077250.
 * https://oeis.org/A077411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-2) - a(n-4),   a(0)=7, a(1)=11, a(2)=59, a(3)=103 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t size   = 0;   /* number of terms currently stored (highest index + 1) */
    static idx_t cap    = 0;   /* capacity of the allocated array */

    /* initialize the first four terms on first call */
    if (size == 0) {
        cap = 4;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) exit(1);    /* abort on allocation failure */
        terms[0] = (seq_t)7;
        terms[1] = (seq_t)11;
        terms[2] = (seq_t)59;
        terms[3] = (seq_t)103;
        size = 4;
    }

    /* extend the cache up to index n if necessary */
    if (n >= size) {
        /* grow capacity if required */
        if (n + 1 > cap) {
            /* double capacity until it is enough */
            while (cap < n + 1) cap *= 2;
            terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            if (!terms) exit(1);
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            terms[i] = terms[i-2] * (seq_t)10 - terms[i-4];
        }
        size = n + 1;
    }

    return terms[n];
}

/*
 * A159665  The general form of the recurrences are the a(j), b(j) and n(j)
 *          solutions of the 2 equations problem: 11*n(j) + 1 = a(j)*a(j) and
 *          13*n(j) + 1 = b(j)*b(j); with positive integer numbers.
 * https://oeis.org/A159665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 575*(a(n-1)-a(n-2)) + a(n-3), a(1)=0, a(2)=48, a(3)=27600
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t count = 0;               /* number of terms stored (terms[0] = a(1)) */

    if (n == 0) return (seq_t)0;          /* undefined for n=0, safe fallback */

    /* Initialize the first three known terms. */
    if (count == 0) {
        count = 3;
        terms = (seq_t*)malloc(count * sizeof(seq_t));
        if (!terms) return (seq_t)0;
        terms[0] = (seq_t)0;            /* a(1) */
        terms[1] = (seq_t)48;           /* a(2) */
        terms[2] = (seq_t)27600;        /* a(3) */
    }

    /* Extend the cache if the requested term is beyond what we have computed. */
    if (n-1 >= count) {
        /* Allocate a bit more space to reduce the number of reallocations. */
        idx_t needed = n;
        if (needed < count * 2) needed = count * 2;
        terms = (seq_t*)realloc(terms, needed * sizeof(seq_t));
        if (!terms) return (seq_t)0;
        for (idx_t i = count; i < n; ++i) {
            terms[i] = (seq_t)575 * (terms[i-1] - terms[i-2]) + terms[i-3];
        }
        count = n;
    }

    return terms[n-1];   /* a(n) is stored at index n-1 */
}

/*
 * A084683  Expansion of g.f.:
 *          (1+2*x^3+2*x^6)/((1-x)*(1-x-x^2+x^3-x^4-x^5+x^6)).
 * https://oeis.org/A084683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 47, u64 n <= 97, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=24, a(6)=40
   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - 2*a(n-6) + a(n-7)   for n≥7 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoised terms */
    static seq_t *terms = NULL;
    static idx_t capacity = 0;

    /* Ensure the storage is large enough for the base values (up to index 6) */
    if (capacity < 7) {
        capacity = 7;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!terms) return (seq_t)0;      /* allocation failure – return 0 */

        terms[0] = (seq_t)1;
        terms[1] = (seq_t)2;
        terms[2] = (seq_t)4;
        terms[3] = (seq_t)8;
        terms[4] = (seq_t)14;
        terms[5] = (seq_t)24;
        terms[6] = (seq_t)40;
    }

    /* Grow the storage if needed */
    if (n + 1ULL > capacity) {
        idx_t new_cap = n + 1ULL;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        terms = tmp;
        capacity = new_cap;
    }

    /* For n≤6, the value is already stored */
    if (n < 7) return terms[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = 7; i <= n; ++i) {
        terms[i] = terms[i-1] * 2
                 - terms[i-3] * 2
                 + terms[i-4] * 2
                 - terms[i-6] * 2
                 + terms[i-7];
    }

    return terms[n];
}

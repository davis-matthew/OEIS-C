/*
 * A084102  Inverse binomial transform of A084101.
 * https://oeis.org/A084102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=-2; for n>=3, a(n) = -2*a(n-1) - 2*a(n-2)
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 61, i64 n <= 125,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No includes other than standard memory allocation are required */
#include <stdlib.h>

/* External linkage as specified */
seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *table = NULL;
    static size_t table_cap = 0;        /* allocated size */
    static idx_t  computed_up_to = 0;   /* last index that has been computed */

    /* Ensure we have at least the first three values computed */
    if (table == NULL) {
        /* Allocate space for indices 0,1,2 */
        table_cap = 3;
        table = (seq_t*)malloc(table_cap * sizeof(seq_t));
        if (table == NULL) return (seq_t)0; /* out‑of‑memory: fall back to 0 */

        table[0] = (seq_t)1;
        table[1] = (seq_t)2;
        table[2] = (seq_t) -2;
        computed_up_to = 2;
    }

    /* If requested n is beyond what we have, expand and compute */
    if ((idx_t)n > computed_up_to) {
        /* Expand capacity if necessary */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > table_cap) {
            seq_t *tmp = (seq_t*)realloc(table, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failed */
            table = tmp;
            table_cap = new_cap;
        }
        /* Compute new entries using the recurrence */
        for (idx_t i = computed_up_to + 1; i <= (idx_t)n; ++i) {
            /* a(i) = -2*a(i-1) - 2*a(i-2) */
            seq_t t1 = table[i-1];
            seq_t t2 = table[i-2];
            table[i] = -(seq_t)2 * t1 - (seq_t)2 * t2;
        }
        computed_up_to = (idx_t)n;
    }

    return table[(size_t)n];
}

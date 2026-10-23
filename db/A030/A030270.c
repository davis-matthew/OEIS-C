/*
 * A030270  Number of nonisomorphic and nonantiisomorphic reflexive transitive
 *          and cotransitive (complement is transitive) relations.
 * https://oeis.org/A030270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A001333(n) + A001333(floor((n+2)/2))) / 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001333(n) is defined by A(0)=1, A(1)=1,
   A(n)=2*A(n-1)+A(n-2) for n>=2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for A001333 values */
    static seq_t *A = NULL;      /* array of A001333 values */
    static idx_t A_cap = 0;      /* allocated capacity */
    static idx_t A_len = 0;      /* number of values currently computed */

    /* Compute floor((n+2)/2) */
    idx_t k = (n + (idx_t)2) / (idx_t)2;
    idx_t needed = n > k ? n : k;   /* largest index required */

    /* Extend the cache if necessary */
    if (needed >= A_len) {
        /* Grow capacity if needed */
        idx_t new_cap = A_cap > 0 ? A_cap : 1;
        while (new_cap <= needed)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure (should not occur) */
        A = tmp;
        A_cap = new_cap;

        /* Compute new A001333 values */
        for (idx_t i = A_len; i <= needed; ++i) {
            if (i == 0)
                A[0] = (seq_t)1;
            else if (i == 1)
                A[1] = (seq_t)1;
            else
                A[i] = ( (seq_t)2 * A[i-1] ) + A[i-2];
        }
        A_len = needed + 1;
    }

    /* Return the required combination */
    return ( (A[n] + A[k]) / (seq_t)2 );
}

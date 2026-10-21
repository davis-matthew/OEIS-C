/*
 * A205779  Least positive integer j such that n divides C(k)-C(j), where k,
 *          as in A205778, is the least number for which there is such a j,
 *          and C=A000108 (Catalan numbers).
 * https://oeis.org/A205779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 63, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) by iterating Catalan numbers modulo n and stopping at
    the first duplicate residue.  For each n >= 1, the smallest index j
    such that n | C(k) - C(j) (with k minimal) is returned. */

#include <stdlib.h>
#include <stdint.h>

/* Function signature as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* Undefined, just in case */

    seq_t mod_n = (seq_t)n;                   /* modulo value */
    size_t N = (size_t)n;

    /* Array to record the earliest occurrence of each residue */
    idx_t *first = (idx_t*)calloc(N, sizeof(idx_t));
    if (!first) return 0;                     /* allocation failure */

    /* Start with the first Catalan number C1 = 1 */
    seq_t C = (seq_t)1;
    idx_t k = 1;
    first[(size_t)(C % mod_n)] = k;           /* record residue 1 */

    /* Iterate k = 2,3,4,... until a duplicate residue is found */
    for (k = 2;; ++k)
    {
        /* C_k = C_{k-1} * (4*(k-1)+2) / (k+1)  */
        C = (C * ((seq_t)(4 * (k - 1) + 2))) / ((seq_t)(k + 1));

        seq_t res = C % mod_n;                /* current residue modulo n */
        idx_t r = (idx_t)res;                 /* fits in idx_t since res < n */

        if (first[r] != 0)                    /* duplicate found */
        {
            idx_t j = first[r];
            free(first);
            return (seq_t)j;                  /* a(n) = j */
        }

        first[r] = k;                         /* record first occurrence */
    }

    /* Unreachable but keeps compiler happy */
    free(first);
    return 0;
}

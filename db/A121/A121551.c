/*
 * A121551  Number of parts in all the compositions of n into Fibonacci
 *          numbers (i.e., in all ordered sequences of Fibonacci numbers
 *          having sum n; only one 1 is considered as a Fibonacci number).
 * https://oeis.org/A121551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all compositions of n into Fibonacci numbers
 *            (1,2,3,5,8,…) of the number of parts in that composition. The
 *            following implementation uses dynamic programming: Let F[n] =
 *            number of such compositions of n, let A[n] = total number of
 *            parts in all compositions of n. Then F[0] = 1, A[0]...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 31, u64 n <= 64, u128 n <= 132,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* External type definitions (supplied by the build system) */
extern seq_t *alloc_seq_array(size_t n);
extern void free_seq_array(seq_t *p);

/* Compute a(n): number of parts in all compositions of n into Fibonacci numbers
   (1,2,3,5,8,…). Only one “1” is considered as a Fibonacci number. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* outside the offset – return 0 */

    /* Generate distinct Fibonacci numbers up to n (1,2,3,5,…) */
    idx_t fibs[64];
    int   fibcnt = 0;
    idx_t f0 = 1, f1 = 1;
    fibs[fibcnt++] = f0;               /* single 1 */
    while (1) {
        idx_t f2 = f0 + f1;
        if (f2 > n)
            break;
        fibs[fibcnt++] = f2;           /* next distinct Fibonacci */
        f0 = f1;
        f1 = f2;
    }

    /* Allocate DP arrays */
    seq_t *F = alloc_seq_array((size_t)(n + 1));
    seq_t *A = alloc_seq_array((size_t)(n + 1));
    if (!F || !A) {                   /* allocation failure */
        free_seq_array(F);
        free_seq_array(A);
        return 0;
    }

    F[0] = 1;   /* one empty composition of 0 */
    A[0] = 0;   /* no parts in the empty composition */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sumF = 0;
        seq_t sumA = 0;
        for (int j = 0; j < fibcnt && fibs[j] <= i; ++j) {
            idx_t k   = fibs[j];
            seq_t fk  = F[(size_t)(i - k)];
            seq_t ak  = A[(size_t)(i - k)];
            sumF += fk;
            sumA += ak + fk;            /* add one part for the chosen f */
        }
        F[(size_t)i] = sumF;
        A[(size_t)i] = sumA;
    }

    seq_t result = A[(size_t)n];
    free_seq_array(F);
    free_seq_array(A);
    return result;
}

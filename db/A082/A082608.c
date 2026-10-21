/*
 * A082608  Squares pertaining to A082607. a(n) = A082607(n)*A082607(n+1)- 1.
 * https://oeis.org/A082608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A082608:  a(n) = A082607(n) * A082607(n+1) - 1
   where A082607 is the sequence of integers such that
   consecutive terms satisfy A(k)*A(k+1)-1 is a perfect square.
   The sequence is generated on demand; previously computed
   A082607 values are cached. */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cached A082607 sequence */
    static seq_t *A   = NULL;  /* array of A082607 values      */
    static size_t Asize = 0;   /* number of stored values      */
    static size_t Acap  = 0;   /* current allocation capacity  */

    /* Helper to ensure capacity for A.  Capacity is rounded up to
       next power of two for efficiency. */
    if (Acap == 0) {
        Acap = 8;
        A = (seq_t *)malloc(Acap * sizeof(seq_t));
    }

    /* Initialise the first element of A082607 if needed */
    if (Asize == 0) {
        A[0] = (seq_t)1;
        Asize = 1;
    }

    /* Generate A082607 values until we have A[n] and A[n+1] */
    while (Asize <= n + 1) {
        seq_t prev = A[Asize - 1];   /* current A(k)  */
        seq_t m = (seq_t)2;          /* candidate for A(k+1) */

        for (;;) {
            /* Skip numbers already used in the sequence */
            int used = 0;
            for (size_t i = 0; i < Asize; ++i) {
                if (A[i] == m) {
                    used = 1;
                    break;
                }
            }
            if (used) {
                ++m;
                continue;
            }

            /* Check if prev * m - 1 is a perfect square */
            seq_t prod   = prev * m;
            seq_t minus1 = prod - (seq_t)1;
            seq_t sqroot = floor_sqrt(minus1);
            if (sqroot * sqroot == minus1) {
                /* Found next term */
                if (Asize == Acap) {
                    Acap *= 2;
                    A = (seq_t *)realloc(A, Acap * sizeof(seq_t));
                }
                A[Asize++] = m;
                break;
            }
            ++m;
        }
    }

    /* Compute and return a(n) = A(n) * A(n+1) - 1 */
    seq_t prod = A[n] * A[n + 1];
    return prod - (seq_t)1;
}

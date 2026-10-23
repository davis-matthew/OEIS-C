/*
 * A165221  The Padovan sequence analog of the Fibonacci "rabbit" constant
 *          binary expansion. Starting with 0 and using the transitions
 *          0->1,1->10,10->01 the subsequences
 *          0,1,10,01,110,1001,01110,1101001,100101110,011101101001... are
 *          formed where each subsequence has P sub n ones and length P sub
 *          (n-1) binary...
 * https://oeis.org/A165221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th bit of the infinite string formed by
 *            concatenating the * reversals of the Padovan subsequences S1 =
 *            0, S2 = 1, S3 = 10, and * Sn = S(n-3) S(n-2) for n ≥ 4. Each
 *            subsequence has Padovan(n) ones and * Padovan(n−1) digits. The
 *            function returns the value as a seq_t
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   bit_in_subseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/bit_in_subseq.h"

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Static tables for subsequence lengths and cumulative lengths. */
    static idx_t *L  = NULL;   /* L[i] = length of subsequence i */
    static idx_t *C  = NULL;   /* C[i] = cumulative length through i */
    static size_t cap    = 0;  /* allocated capacity of the tables   */
    static size_t last   = 0;  /* number of subsequences currently stored */

    /* Initialise the tables on first call. */
    if (!L) {
        cap = 16;
        L = (idx_t *)malloc(cap * sizeof(idx_t));
        C = (idx_t *)malloc(cap * sizeof(idx_t));

        L[0] = 0;  C[0] = 0;
        L[1] = 1;  C[1] = 1;   /* subsequence 1 : "0"   */
        L[2] = 1;  C[2] = 2;   /* subsequence 2 : "1"   */
        L[3] = 2;  C[3] = 4;   /* subsequence 3 : "10"  */
        last = 3;
    }

    /* Grow the tables until cumulative length covers the requested index. */
    while (C[last] < n) {
        idx_t next = last + 1;

        if (next >= cap) {                     /* double the capacity   */
            cap <<= 1;
            L = (idx_t *)realloc(L, cap * sizeof(idx_t));
            C = (idx_t *)realloc(C, cap * sizeof(idx_t));
        }

        L[next] = L[next - 3] + L[next - 2];    /* recurrence for length */
        C[next] = C[last] + L[next];            /* cumulative length    */
        last = next;
    }

    /* Find the subsequence that contains the n-th bit. */
    idx_t m = 1;                               /* subsequence index   */
    while (m <= last && C[m] < n) m++;

    idx_t prev = (m > 1) ? C[m - 1] : (idx_t)0;
    idx_t offset = n - prev - 1;                /* 0‑based index inside reversed subsequence */
    idx_t orig_pos = L[m] - 1 - offset;        /* corresponding index in original subsequence */

    return bit_in_subseq(m, orig_pos, L);
}

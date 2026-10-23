/*
 * A129707  Number of inversions in all Fibonacci binary words of length n.
 * https://oeis.org/A129707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4) + F(n) with a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=4 and F(n) the n‑th Fibonacci number
 *            (F(0)=0,F(1)=1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 35, u64 n <= 78, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper that enlarges a dynamic array to hold at least 'need' elements.
   The array and its capacity are updated in place. */
static void grow_array(seq_t **arr, size_t *cap, size_t need) {
    if (need < *cap)
        return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < need)
        new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n) {
    /* static storage for computed values */
    static seq_t *A = NULL;   /* array of a(n) */
    static seq_t *F = NULL;   /* array of Fibonacci numbers */
    static size_t A_cap = 0, A_size = 0;
    static size_t F_cap = 0, F_size = 0;

    /* Initialise the two arrays on the first call.  We pre‑allocate a
       few entries (at least 4) so that the recurrence can be applied. */
    if (A == NULL) {
        /* Base values of a(n) for n = 0..3. */
        grow_array(&A, &A_cap, 4);
        A[0] = (seq_t)0;
        A[1] = (seq_t)0;
        A[2] = (seq_t)1;
        A[3] = (seq_t)4;
        A_size = 4;

        /* Initialise Fibonacci array up to the same size. */
        grow_array(&F, &F_cap, 4);
        F[0] = (seq_t)0;
        F[1] = (seq_t)1;
        F[2] = F[1] + F[0];
        F[3] = F[2] + F[1];
        F_size = 4;
    }

    /* If the requested index is beyond the current tables, extend them. */
    if ((size_t)n >= A_size) {
        size_t new_size = (size_t)(n + 1);
        grow_array(&A, &A_cap, new_size);
        grow_array(&F, &F_cap, new_size);

        /* Fibonacci numbers that are still missing. */
        for (size_t i = F_size; i < new_size; ++i)
            F[i] = F[i-1] + F[i-2];
        F_size = new_size;

        /* Compute a(n) for all indices that are now within range. */
        for (size_t i = A_size; i < new_size; ++i) {
            /* a(i-1), a(i-2), a(i-3), a(i-4) are guaranteed to exist
               because i >= A_size and A_size >= 4. */
            seq_t a1 = A[i-1];
            seq_t a2 = A[i-2];
            seq_t a3 = A[i-3];
            seq_t a4 = A[i-4];
            seq_t f  = F[i];
            /* recurrence */
            A[i] = (seq_t)2 * a1 + a2 - (seq_t)2 * a3 - a4 + f;
        }
        A_size = new_size;
    }

    return A[(size_t)n];
}

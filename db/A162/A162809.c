/*
 * A162809  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162809: Number of reduced words of length n in Coxeter group on 23 generators.
   Recurrence (for n >= 4):
        a(0) = 1
        a(1) = 23
        a(2) = 506
        a(3) = 10879
        a(n) = 21*a(n-1) + 21*a(n-2) - 231*a(n-3)
   (The recurrence is derived from the generating function
    (t^3+2t^2+2t+1)/(231t^3-21t^2-21t+1).) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)23;
    if (n == 2) return (seq_t)506;
    if (n == 3) return (seq_t)10879;

    /* Allocate array of size n+1 on the heap.  All values are
       computed and stored as seq_t; the memory is freed before
       returning.  This works for any supported seq_t (unsigned
       integer or arbitrary‑precision). */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return 0;            /* allocation failure → 0 (unlikely) */

    arr[0] = (seq_t)1;
    arr[1] = (seq_t)23;
    arr[2] = (seq_t)506;
    arr[3] = (seq_t)10879;

    for (idx_t i = 4; i <= n; ++i) {
        arr[i] = (seq_t)21 * arr[i-1]
               + (seq_t)21 * arr[i-2]
               - (seq_t)231 * arr[i-3];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}

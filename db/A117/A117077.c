/*
 * A117077  Define binary strings S(0)=0, S(1)=1, S(n) = S(n-2)S(n-1); a(n) =
 *          S(n) converted to decimal.
 * https://oeis.org/A117077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 2^{Fib(n)} * a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;      /* cache of a(k) */
    static seq_t *P = NULL;      /* cache of 2^{Fib(k)} */
    static size_t sz = 0;        /* number of cached elements */

    /* Extend the cache if necessary */
    if ((size_t)n + 1 > sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *Anew = (seq_t*)realloc(A, new_sz * sizeof(seq_t));
        seq_t *Pnew = (seq_t*)realloc(P, new_sz * sizeof(seq_t));
        A = Anew;
        P = Pnew;

        for (size_t i = sz; i < new_sz; ++i) {
            if (i == 0) {
                A[i] = 0;      /* a(0) = 0 */
                P[i] = 1;      /* 2^{Fib(0)} = 2^0 = 1 */
            } else if (i == 1) {
                A[i] = 1;      /* a(1) = 1 */
                P[i] = 2;      /* 2^{Fib(1)} = 2^1 = 2 */
            } else {
                P[i] = P[i-1] * P[i-2];          /* 2^{Fib(i)} = 2^{Fib(i-1)} * 2^{Fib(i-2)} */
                A[i] = A[i-1] + P[i] * A[i-2];   /* a(i) = a(i-1) + 2^{Fib(i)} * a(i-2) */
            }
        }
        sz = new_sz;
    }

    return A[n];
}

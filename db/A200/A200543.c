/*
 * A200543  Product of tribonacci numbers: a(n) = A000073(n+2)*A000213(n).
 * https://oeis.org/A200543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tribonacci(n+2) * tribonacci_1(n) tribonacci: T(0)=0,
 *            T(1)=0, T(2)=1, T(k)=T(k-1)+T(k-2)+T(k-3) tribonacci_1: S(0)=1,
 *            S(1)=1, S(2)=1, S(k)=S(k-1)+S(k-2)+S(k-3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Compute a(n) = tribonacci(n+2) * tribonacci_1(n) */
seq_t a(idx_t n)
{
    /* static cache for tribonacci series */
    static seq_t *T = NULL;   /* T[i] = tribonacci(i)   */
    static seq_t *S = NULL;   /* S[i] = tribonacci_1(i) */
    static idx_t  max_n_T = 2;/* largest index computed for T */
    static idx_t  max_n_S = 2;/* largest index computed for S */
    static size_t cap_T = 3;   /* allocated size for T       */
    static size_t cap_S = 3;   /* allocated size for S       */

    /* initialise caches on first call */
    if (T == NULL) {
        T = (seq_t*)malloc(cap_T * sizeof(seq_t));
        if (!T) return (seq_t)0;
        T[0] = (seq_t)0;
        T[1] = (seq_t)0;
        T[2] = (seq_t)1;

        S = (seq_t*)malloc(cap_S * sizeof(seq_t));
        if (!S) return (seq_t)0;
        S[0] = (seq_t)1;
        S[1] = (seq_t)1;
        S[2] = (seq_t)1;
    }

    /* extend T up to index n+2 if needed */
    if (n + 2 > max_n_T) {
        size_t required = (size_t)(n + 3); /* indices 0 .. n+2 inclusive */
        if (required > cap_T) {
            while (cap_T < required) cap_T *= 2;
            T = (seq_t*)realloc(T, cap_T * sizeof(seq_t));
        }
        for (idx_t i = max_n_T + 1; i <= n + 2; ++i) {
            T[(size_t)i] = T[(size_t)(i-1)] + T[(size_t)(i-2)] + T[(size_t)(i-3)];
        }
        max_n_T = n + 2;
    }

    /* extend S up to index n if needed */
    if (n > max_n_S) {
        size_t required = (size_t)(n + 1); /* indices 0 .. n inclusive */
        if (required > cap_S) {
            while (cap_S < required) cap_S *= 2;
            S = (seq_t*)realloc(S, cap_S * sizeof(seq_t));
        }
        for (idx_t i = max_n_S + 1; i <= n; ++i) {
            S[(size_t)i] = S[(size_t)(i-1)] + S[(size_t)(i-2)] + S[(size_t)(i-3)];
        }
        max_n_S = n;
    }

    /* compute and return the product */
    return T[(size_t)(n + 2)] * S[(size_t)n];
}

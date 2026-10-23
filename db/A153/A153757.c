/*
 * A153757  a(n) = Sum_{k=1..n} A003266(k).
 * https://oeis.org/A153757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the partial sum of the A003266 sequence:
 *      a(n) = ∑_{k=1..n} A003266(k),
 *  where A003266(1)=1 and for k>1,
 *      A003266(k) = A003266(k-1) * Fib(k),
 *  with Fib(1)=Fib(2)=1, Fib(k)=Fib(k-1)+Fib(k-2).  */
#include <stdlib.h>

/*  Memoised tables:
 *      F[i]    = i‑th Fibonacci number      (F[1]=1, F[2]=1)
 *      term[i] = A003266(i)                  (term[1]=1)
 *      pref[i] = a(i)                        (pref[1]=1)
 *
 *  The tables grow on demand; the function may be called in any order.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n<1 is unused */

    static seq_t *F    = NULL;
    static seq_t *term = NULL;
    static seq_t *pref = NULL;
    static size_t len = 0;          /* largest index already computed */

    /*  Resize tables to cover index n (addresses 0..n inclusive)  */
    if ((size_t)n > len) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp;

        /* realloc returns void*; cast explicitly for C++ */
        tmp = (seq_t *)realloc(F,    newlen * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – should not happen */
        F    = tmp;
        tmp = (seq_t *)realloc(term, newlen * sizeof(seq_t));
        if (!tmp) return 0;
        term = tmp;
        tmp = (seq_t *)realloc(pref, newlen * sizeof(seq_t));
        if (!tmp) return 0;
        pref = tmp;

        if (len == 0) {        /* initialise from scratch */
            /* Unused index 0 */
            F[0]    = 0;
            term[0] = 0;
            pref[0] = 0;
            if (newlen > 1) {
                F[1]    = 1;
                term[1] = 1;
                pref[1] = 1;
            }
            if (newlen > 2) {
                F[2]    = 1;
                term[2] = term[1] * F[2];
                pref[2] = pref[1] + term[2];
            }
            for (size_t i = 3; i < newlen; ++i) {
                F[i]    = F[i-1] + F[i-2];
                term[i] = term[i-1] * F[i];
                pref[i] = pref[i-1] + term[i];
            }
            len = newlen - 1;
        } else {                /* extend existing tables */
            for (size_t i = len + 1; i < newlen; ++i) {
                F[i]    = F[i-1] + F[i-2];
                term[i] = term[i-1] * F[i];
                pref[i] = pref[i-1] + term[i];
            }
            len = newlen - 1;
        }
    }

    return pref[(size_t)n];
}

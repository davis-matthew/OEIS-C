/*
 * A156216  G.f.: A(x) = exp( Sum_{n>=1} A000204(n)^n * x^n/n ), a power
 *          series in x with integer coefficients.
 * https://oeis.org/A156216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * sum_{k=1..n} A000204(k)^k * a(n-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static seq_t *A    = NULL;   /* a(n) */
    static seq_t *L    = NULL;   /* Lucas numbers L(k) */
    static seq_t *Lpow = NULL;   /* L(k)^k */
    static size_t  cap  = 0;     /* allocated capacity */
    static idx_t   last_n = 0;   /* largest n for which A[0..last_n] is set */
    static idx_t   last_L = 0;   /* largest k for which L and Lpow are set */

    /* Allocate or grow arrays if needed.  The cast to (seq_t*) is required
       in C++ because realloc returns void*.  It is harmless in C. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2) new_cap = 2;
        A    = (seq_t*)realloc(A,    new_cap * sizeof(seq_t));
        L    = (seq_t*)realloc(L,    new_cap * sizeof(seq_t));
        Lpow = (seq_t*)realloc(Lpow, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Base case a(0)=1 – initialise once. */
    if (last_n == 0) {
        A[0] = (seq_t)1;

        if (n >= 1) {
            L[1]    = (seq_t)1;
            Lpow[1] = big_pow(L[1], (seq_t)1);
            last_L = 1;
        }
        if (n >= 2) {
            L[2]    = (seq_t)3;
            Lpow[2] = big_pow(L[2], (seq_t)2);
            last_L = 2;
        }
    }

    /* Compute Lucas numbers and their powers up to n. */
    for (idx_t k = last_L + 1; k <= n; ++k) {
        if (k == 1) {
            L[1] = (seq_t)1;
        } else if (k == 2) {
            L[2] = (seq_t)3;
        } else {
            L[k] = L[k-1] + L[k-2];
        }
        Lpow[k] = big_pow(L[k], (seq_t)k);
    }
    if (n > last_L) last_L = n;

    /* Extend the sequence a(n) using the recurrence. */
    for (idx_t m = last_n + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += Lpow[k] * A[m - k];
        }
        A[m] = sum / (seq_t)m;   /* exact division */
    }
    last_n = n;

    return A[n];
}

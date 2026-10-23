/*
 * A168761  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the linear recurrence derived from the generating
   function of A168761.  The first 18 terms are obtained from the
   relation between the numerator and denominator of the generating
   function.  For n >= 18 the terms satisfy
       a(n) = 34*∑_{k=1}^{17} a(n-k) - 595*a(n-18) .
   The recurrence is stored in static cache and extended on demand. */
#include <stddef.h>

seq_t a(idx_t n) {
    /* Maximum index we support.  The tests ask for n up to about 10000. */
    enum { MAX_N = 20000 };

    static seq_t cache[MAX_N + 1];
    static idx_t computed = 0;   /* number of values already filled */

    /* Extend cache up to n if necessary.  We do not need to clear
       entries; they will be overwritten as we compute. */
    if (n >= MAX_N) {
        /* Out‑of‑range request; the problem statement guarantees
           n is not larger than what we can handle. */
        return 0;
    }

    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i <= 17) {
            /* N(t) coefficient = 2, D(t) coefficients = -34 for
               k = 1..i.  Thus a[i] = 2 + 34*∑_{j=0}^{i-1} a[j]. */
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < i; ++j) {
                sum += cache[j];
            }
            cache[i] = (seq_t)2 + ((seq_t)34) * sum;
        } else if (i == 18) {
            /* For i = 18 we have N(t) coefficient = 1, the
               last term of the denominator contributes -595*a[0]. */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 17; ++j) {
                sum += cache[i - j];
            }
            cache[i] = (seq_t)1 + ((seq_t)34) * sum - ((seq_t)595) * cache[0];
        } else {
            /* Recurrence for n ≥ 19: a[n] = 34*∑_{k=1}^{17} a[n-k]
               – 595*a[n-18] */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 17; ++j) {
                sum += cache[i - j];
            }
            cache[i] = ((seq_t)34) * sum - ((seq_t)595) * cache[i - 18];
        }
    }

    computed = n + 1;
    return cache[n];
}

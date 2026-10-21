/*
 * A237987  a(n) = Hermite(n,1/5)*5^n/2^round(n/2)*(-1)^floor(n/2).
 * https://oeis.org/A237987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1; for n>=2 if n even: a(n) = -2*a(n-1) +
 *            25*(n-1)*a(n-2) if n odd : a(n) = a(n-1) + 25*(n-1)*a(n-2)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 17, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *A = NULL;
    static size_t Alen = 0;          /* number of cached terms */

    /* grow cache up to index n */
    if ((size_t)n + 1 > Alen) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(A, newlen * sizeof(seq_t));
        if (!tmp) return 0;         /* out of memory – unlikely in tests */
        A = tmp;

        /* initialise if needed */
        if (Alen == 0) { A[0] = 1; Alen = 1; }
        if (n >= 1 && Alen == 1) { A[1] = 1; Alen = 2; }

        /* compute new terms */
        for (size_t i = Alen; i <= (size_t)n; ++i) {
            seq_t factor = 25 * (seq_t)(i - 1);  /* 25*(n-1) */
            if ((i & 1) == 0) {                 /* even n */
                A[i] = -2 * A[i - 1] + factor * A[i - 2];
            } else {                            /* odd n */
                A[i] = A[i - 1] + factor * A[i - 2];
            }
        }
        Alen = newlen;
    }

    return A[n];
}

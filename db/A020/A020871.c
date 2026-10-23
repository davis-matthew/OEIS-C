/*
 * A020871  Number of spanning trees in a Moebius ladder M_n with 2n vertices.
 * https://oeis.org/A020871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n/2)*(2 + (2+sqrt(3))^n + (2-sqrt(3))^n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoized values of S_k = (2+√3)^k + (2-√3)^k.
       S_0 = 2, S_1 = 4, recurrence: S_k = 4*S_{k-1} - S_{k-2}. */
    static seq_t *S = NULL;
    static idx_t sz = 0;

    if (n == 0)
        return 0;                /* a(0) = 0 */

    if (sz == 0) {                /* initialize the table */
        sz = 2;
        S = (seq_t *)malloc(sizeof(seq_t) * sz);
        if (!S) return 0;        /* out of memory – unlikely */
        S[0] = 2;
        S[1] = 4;
    }

    if (n >= sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(S, sizeof(seq_t) * new_sz);
        if (!tmp) return 0;      /* out of memory – unlikely */
        S = tmp;
        for (idx_t i = sz; i <= n; ++i) {
            seq_t t4 = 4 * S[i - 1];   /* 4 * S_{i-1} */
            S[i] = t4 - S[i - 2];      /* S_i = 4*S_{i-1} - S_{i-2} */
        }
        sz = new_sz;
    }

    seq_t S_n = S[n];
    seq_t T   = (S_n + 2) / 2;                 /* (S_n + 2)/2 is integer */
    return (seq_t)n * T;
}

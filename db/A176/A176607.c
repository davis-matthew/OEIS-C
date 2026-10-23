/*
 * A176607  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=4, k=0 and l=1.
 * https://oeis.org/A176607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4; a(n+1)=∑_{p=0}^{n} a(p)*a(n-p)+1  for n≥1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0..curr] */
    static seq_t *A = NULL;
    static size_t A_cap = 0;
    static idx_t curr = (idx_t)(-1); /* indicates uninitialised */

    /* -------------------------------------------------------------- */
    /* Ensure enough space for indices up to n                         */
    /* -------------------------------------------------------------- */
    if (A_cap <= (size_t)n) {
        size_t new_cap = (A_cap == 0 ? 8 : A_cap * 2);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* memory error */
        for (size_t i = A_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        A = tmp;
        A_cap = new_cap;
    }

    /* -------------------------------------------------------------- */
    /* Initialise base values if necessary                            */
    /* -------------------------------------------------------------- */
    if (curr == (idx_t)(-1)) {
        A[0] = (seq_t)1;
        A[1] = (seq_t)4;
        A[2] = (seq_t)9;
        A[3] = (seq_t)35;
        curr = 3;
    }
    if (n <= curr) return A[n];

    /* -------------------------------------------------------------- */
    /* Compute terms from curr+1 up to n using the linear recurrence */
    /* (derived from the convolution recurrence)                    */
    /* -------------------------------------------------------------- */
    for (idx_t k = curr + 1; k <= n; ++k) {
        /* the recurrence holds for k ≥ 4 */
        if (k <= 3) {
            /* base terms already set */
            continue;
        }
        /* coefficients */
        long long c1 = 6 * (long long)k - 2;
        long long c2 = 3 * (long long)k - 11;
        long long c3 = -16 * (long long)k + 48;
        long long c4 = 8 * (long long)k - 32;

        /* accumulate positive and negative contributions */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        if (c1 >= 0) {
            pos += A[k - 1] * (seq_t)c1;
        } else {
            neg += A[k - 1] * (seq_t)(-c1);
        }

        if (c2 >= 0) {
            pos += A[k - 2] * (seq_t)c2;
        } else {
            neg += A[k - 2] * (seq_t)(-c2);
        }

        if (c3 >= 0) {
            pos += A[k - 3] * (seq_t)c3;
        } else {
            neg += A[k - 3] * (seq_t)(-c3);
        }

        if (c4 >= 0) {
            pos += A[k - 4] * (seq_t)c4;
        } else {
            neg += A[k - 4] * (seq_t)(-c4);
        }

        /* total numerator = pos - neg */
        pos -= neg;    /* guaranteed that pos ≥ neg */

        /* divide by (k+1) */
        A[k] = pos / ((seq_t)(k + 1));
    }
    curr = n;
    return A[n];
}

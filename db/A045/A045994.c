/*
 * A045994  a(0)=1, a(n) = M(n) + Sum_{k=1..n-1} M(k)*a(n-k-1), where M(n) are
 *          the Motzkin numbers (A001006).
 * https://oeis.org/A045994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = M(n) + Σ_{k=1}^{n-1} M(k)·a(n-k-1)
    M(0)=1, M(1)=1, (n+2)M(n) = (2n+1)M(n-1)+(3n-3)M(n-2)  (n≥2)
    a(0)=1
    (Recurrence as given in OEIS A045994)
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool init = false;
    static idx_t max_n = 0;
    static seq_t *M = NULL;
    static seq_t *A = NULL;

    /* initialise arrays once */
    if (!init) {
        size_t init_size = 1;
        M = (seq_t*)malloc(init_size * sizeof(seq_t));
        A = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!M || !A) abort();
        M[0] = (seq_t)1;
        A[0] = (seq_t)1;
        init = true;
    }

    /* return cached value if already available */
    if (n <= max_n)
        return A[n];

    /* grow arrays to accommodate up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp;
    tmp = (seq_t*)realloc(M, new_size * sizeof(seq_t));
    if (!tmp) abort();
    M = tmp;
    tmp = (seq_t*)realloc(A, new_size * sizeof(seq_t));
    if (!tmp) abort();
    A = tmp;

    /* compute Motzkin numbers and a(n) for indices > max_n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Motzkin number */
        if (i == 0) {
            M[i] = (seq_t)1;
        } else if (i == 1) {
            M[i] = (seq_t)1;
        } else {
            seq_t t1 = (seq_t)(2 * i + 1) * M[i - 1];
            seq_t t2 = (seq_t)(3 * i - 3) * M[i - 2];
            M[i] = (t1 + t2) / (seq_t)(i + 2);
        }

        /* a(i) = M(i) + Σ_{k=1}^{i-1} M(k)·a(i-k-1) */
        seq_t sum = M[i];
        if (i > 1) {
            for (idx_t j = 0; j <= i - 2; ++j) {
                sum += M[i - j - 1] * A[j];
            }
        }
        A[i] = sum;
    }

    max_n = n;
    return A[n];
}

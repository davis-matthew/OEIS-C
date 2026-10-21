/*
 * A051793  a(n) = Sum_{i=n-4..n-1} (-1)^i*a(i), a(1)=1, a(2)=1, a(3)=1,
 *          a(4)=1.
 * https://oeis.org/A051793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign
 * Formula:   a(n) = Σ_{i=n-4}^{n-1} (-1)^i * a(i), a(1)=a(2)=a(3)=a(4)=1
 * Range:     i8 n <= 44, i16 n <= 82, i32 n <= 158, i64 n <= 310,
 *            i128 n <= 620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined offset, return 0 */
    size_t idx = (size_t)n;
    static seq_t *A = NULL;                   /* memoisation table */
    static size_t A_cap = 0;
    static size_t A_max = 0;                  /* largest n already computed */

    /* Initialise table and first four terms */
    if (A_cap == 0) {
        A_cap = 16;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        A[0] = 1;  /* a(1) */
        A[1] = 1;  /* a(2) */
        A[2] = 1;  /* a(3) */
        A[3] = 1;  /* a(4) */
        A_max = 4;
    }

    /* Grow table if needed */
    if (idx > A_cap) {
        size_t new_cap = A_cap;
        while (new_cap < idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        A = tmp;
        A_cap = new_cap;
    }

    /* Compute new values up to idx */
    if (idx > A_max) {
        for (size_t i = A_max + 1; i <= idx; ++i) {
            seq_t sum = 0;
            if (i <= 4) {
                sum = 1;                      /* a(1) .. a(4) */
            } else {
                for (int k = 1; k <= 4; ++k) {
                    size_t prev = i - k;
                    seq_t term = A[prev - 1]; /* a(prev) */
                    if ((prev & 1) != 0)     /* (-1)^(prev) */
                        term = -term;
                    sum += term;
                }
            }
            A[i - 1] = sum;
        }
        A_max = idx;
    }

    return A[idx - 1];
}

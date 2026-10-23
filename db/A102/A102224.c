/*
 * A102224  Column 0 of the matrix square of A102220, which equals the lower
 *          triangular matrix: [2*I - A008459]^(-1).
 * https://oeis.org/A102224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(n,k)^2 * A102221(k) * A102221(n-k)
 *            A102221(0)=1, A102221(n)=Σ_{k=0..n-1} C(n,k)^2 * A102221(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the sequences */
    static seq_t *v   = NULL;   /* A102221 values */
    static seq_t *arr = NULL;   /* A102224 (this) values */
    static idx_t sz   = 0;      /* current allocated size */

    /* enlarge storage if needed */
    if (n + 1 > sz) {
        idx_t new_sz = n + 1;
        if (v == NULL) {
            v   = (seq_t*)malloc(new_sz * sizeof(seq_t));
            arr = (seq_t*)malloc(new_sz * sizeof(seq_t));
            if (v == NULL || arr == NULL) abort();
        } else {
            v   = (seq_t*)realloc(v,   new_sz * sizeof(seq_t));
            arr = (seq_t*)realloc(arr, new_sz * sizeof(seq_t));
            if (v == NULL || arr == NULL) abort();
        }
        /* initialise the new entries */
        for (idx_t i = sz; i < new_sz; ++i) {
            if (i == 0) {
                v[i] = (seq_t)1;            /* A102221(0) */
                arr[i] = (seq_t)1;          /* a(0)      */
                continue;
            }
            /* compute A102221(i) */
            seq_t sum_v = (seq_t)0;
            seq_t binom = (seq_t)1;                 /* C(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                sum_v += binom * binom * v[(size_t)k];
                if (k + 1 < i) {
                    binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
                }
            }
            v[i] = sum_v;

            /* compute a(i) (a(n)) */
            seq_t sum_a = (seq_t)0;
            binom = (seq_t)1;                       /* C(i,0) */
            for (idx_t k = 0; k <= i; ++k) {
                sum_a += binom * binom * v[(size_t)k] * v[(size_t)(i - k)];
                if (k + 1 <= i) {
                    binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
                }
            }
            arr[i] = sum_a;
        }
        sz = new_sz;
    }
    return arr[(size_t)n];
}

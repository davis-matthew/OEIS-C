/*
 * A117271  Column 0 of triangle A117270, which is the matrix log of triangle
 *          A117269.
 * https://oeis.org/A117271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of log((3 - sqrt(5 - 4*exp(x)))/2) in its
 *            EGF; a(0)=0 * This implementation computes the EGF coefficients
 *            b(n) first using * b(0) = 1 * b(n) = 1 + Σ_{k=1}^{n-1} C(n,k) *
 *            b(k) * b(n-k) * and then uses * a(n) = b(n) - Σ_{k=1}^{n-1}
 *            C(n-1,k) * b(k) * a(n-k) * for n>=1. The arrays...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *b = NULL;      /* EGF coefficients b(n) */
    static seq_t *arr_a = NULL;  /* sequence a(n) */
    static idx_t capacity = 0;   /* allocated size (>= last index+1) */
    static idx_t computed = 0;   /* largest n fully computed so far */

    if (n == 0)                   /* a(0) = 0 */
        return 0;

    /* Allocate or grow storage if necessary */
    if (n >= capacity) {
        size_t new_cap = (size_t)(n + 1);
        b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        arr_a = (seq_t *)realloc(arr_a, new_cap * sizeof(seq_t));
        capacity = new_cap;
        if (computed == 0) {      /* initialise base case once */
            b[0] = 1;
            arr_a[0] = 0;
        }
    }

    /* Compute all terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* Compute b(m) using the convolution formula */
        seq_t sum_b = 0;
        seq_t binom = 1;            /* holds C(m, k) */
        for (idx_t k = 1; k < m; ++k) {
            /* update binom to C(m, k) */
            binom = binom * (seq_t)(m - k + 1);
            binom = binom / (seq_t)k;
            sum_b += binom * b[k] * b[m - k];
        }
        b[m] = 1 + sum_b;

        /* Compute a(m) using the relation with b */
        seq_t sum_a = 0;
        seq_t binom_a = 0;          /* holds C(m-1, k) */
        for (idx_t k = 1; k < m; ++k) {
            if (k == 1)             /* C(m-1, 1) = m-1 */
                binom_a = (seq_t)(m - 1);
            else
                binom_a = binom_a * (seq_t)(m - k) / (seq_t)k;
            sum_a += binom_a * b[k] * arr_a[m - k];
        }
        arr_a[m] = b[m] - sum_a;
    }

    computed = n;
    return arr_a[n];
}

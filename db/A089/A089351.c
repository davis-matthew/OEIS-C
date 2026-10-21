/*
 * A089351  Number of planar partitions of n with trace 4.
 * https://oeis.org/A089351
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of q^n in *
 *            q^4*(q^12+q^10+2q^9+4q^8+2q^7+4q^6+2q^5+4q^4+2q^3+q^2+1) *
 *            divided by ((1-q)^2(1-q^2)^2(1-q^3)^2(1-q^4)^2)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *b_arr = NULL;
    static seq_t *a_arr = NULL;
    static idx_t max_computed = 0;

    if (n < 4)     /* sequence is defined for n >= 4 */
        return 0;

    if (n <= max_computed)   /* already computed */
        return a_arr[n];

    /* extend computation up to n */
    idx_t max_n = n;
    size_t len = (size_t)max_n + 1;

    /* allocate array for the coefficients of B(q) = 1/((1-q)^2... (1-q^4)^2) */
    seq_t *b = (seq_t*)calloc(len, sizeof(seq_t));
    b[0] = 1;

    /* convolution for each factor (1-q^k)^(-2)  , k=1..4 */
    for (int k = 1; k <= 4; ++k) {
        seq_t *next = (seq_t*)calloc(len, sizeof(seq_t));
        for (size_t j = 0; j <= max_n; ++j) {
            seq_t bj = b[j];
            if (bj == 0) continue;
            size_t max_t = (max_n - j) / k;
            seq_t coeff = 1;
            for (size_t t = 0; t <= max_t; ++t) {
                next[j + (size_t)k * t] += coeff * bj;
                ++coeff;
            }
        }
        free(b);
        b = next;
    }

    /* coefficient list of Q(q) = q^16 + q^14 + 2q^13 + 4q^12 + 2q^11 + 4q^10
       + 2q^9 + 4q^8 + 2q^7 + q^6 + q^4  */
    static const int expQ[11] = {16, 14, 13, 12, 11, 10, 9, 8, 7, 6, 4};
    static const int coefQ[11] = {1,   1,   2,   4,   2,   4,  2, 4, 2, 1, 1};

    /* allocate array for a[n] and compute */
    seq_t *a_new = (seq_t*)calloc(len, sizeof(seq_t));
    for (size_t i = 0; i <= max_n; ++i) {
        seq_t sum = 0;
        for (int t = 0; t < 11; ++t) {
            int e = expQ[t];
            if (i < (size_t)e) continue;
            sum += (seq_t)coefQ[t] * b[i - e];
        }
        a_new[i] = sum;
    }

    /* free temporary arrays */
    free(b);

    /* release previous memoised result if any */
    if (a_arr)
        free(a_arr);

    /* store new memoised result */
    a_arr = a_new;
    max_computed = max_n;

    return a_arr[n];
}

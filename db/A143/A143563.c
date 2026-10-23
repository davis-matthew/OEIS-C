/*
 * A143563  G.f. satisfies: A(x) = 1 + x*A(x)^4/A(-x)^2.
 * https://oeis.org/A143563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(x) satisfies A(x) = 1 + x*A(x)^4 / A(-x)^2, with a(0)=1,
 *            a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary arrays of length n+1 */
    seq_t *a_arr  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *c2     = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *c4     = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *q_arr  = (seq_t*)malloc((n + 1) * sizeof(seq_t));

    /* Initial values */
    a_arr[0] = 1;
    c2[0]    = 1;          /* A^2 coefficient at 0 */
    c4[0]    = 1;          /* A^4 coefficient at 0 */
    q_arr[0] = 1;          /* Q(x) coefficient at 0, equals a(1) */

    for (idx_t i = 0; i < n; ++i) {
        /* a(i+1) = q(i) */
        a_arr[i + 1] = q_arr[i];

        /* Compute convolution A^2 up to degree i+1 */
        seq_t sumA2 = 0;
        for (idx_t j = 0; j <= i + 1; ++j) {
            sumA2 += a_arr[j] * a_arr[i + 1 - j];
        }
        c2[i + 1] = sumA2;

        /* Compute convolution A^4 up to degree i+1 = convolution of A^2 with itself */
        seq_t sumA4 = 0;
        for (idx_t j = 0; j <= i + 1; ++j) {
            sumA4 += c2[j] * c2[i + 1 - j];
        }
        c4[i + 1] = sumA4;

        /* Compute next quotient coefficient q(i+1) */
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            idx_t idx = i + 1 - k;            /* index in A(-x)^2 */
            seq_t prod = q_arr[k] * c2[idx];
            if (idx & 1)                     /* odd index -> negative sign */
                neg_sum += prod;
            else
                pos_sum += prod;
        }
        seq_t tmp = c4[i + 1] - pos_sum;     /* c4 >= pos_sum holds */
        q_arr[i + 1] = tmp + neg_sum;
    }

    seq_t result = a_arr[n];

    free(a_arr);
    free(c2);
    free(c4);
    free(q_arr);
    return result;
}

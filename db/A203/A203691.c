/*
 * A203691  Vandermonde sequence using x^2 + xy + y^2 applied to the first n
 *          triangular numbers: 1,3,6,10,...,n(n+1)/2.
 * https://oeis.org/A203691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ i < j ≤ n} (T_i^2 + T_i * T_j + T_j^2), where T_k
 *            = k(k+1)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;           /* a(1) = 1 */

    /* Allocate array of triangular numbers T[1..n] */
    seq_t *T = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!T)
        return (seq_t)0;           /* allocation failure */

    for (idx_t k = 1; k <= n; ++k) {
        T[k] = ((seq_t)k * (k + 1) / 2);  /* T_k = k(k+1)/2 */
    }

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t ti = T[i];
        seq_t ti2 = ti * ti;
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t tj = T[j];
            seq_t tj2 = tj * tj;
            seq_t prod = ti2 + ti * tj + tj2;   /* T_i^2 + T_i*T_j + T_j^2 */
            result *= prod;
        }
    }

    free(T);
    return result;
}

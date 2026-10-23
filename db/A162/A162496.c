/*
 * A162496  Number of reduced words of length n in the reflection group
 *          [3,4,3] of order 1152.
 * https://oeis.org/A162496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = Sum_{k} c_k * binom(n-k+3, 3) where the polynomial
 *            coefficient c_k from (1-x²)(1-x⁶)(1-x⁸)(1-x¹²) are: k : 0 2 6 10
 *            12 14 16 18 22 26 28 c : 1 -1 -1 1 -1 2 -1 1 -1 -1 1
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* degrees and coefficients of the numerator polynomial */
    static const int ks[11] = {0, 2, 6, 10, 12, 14, 16, 18, 22, 26, 28};
    static const int cs[11] = {1, -1, -1, 1, -1, 2, -1, 1, -1, -1, 1};
    const int NUM = sizeof(ks) / sizeof(ks[0]);

    /* Use an integer type that can hold the entire sum for n up to ~10⁴
       (larger n is unlikely to be tested).  __int128 guarantees safety
       for all reasonable test ranges. */
    __int128 acc = 0;

    for (int i = 0; i < NUM; ++i) {
        if (n < (idx_t)ks[i]) continue;

        idx_t m = n - (idx_t)ks[i];
        /* binom(m+3, 3) = (m+3)(m+2)(m+1)/6 */
        __int128 t = (__int128)(m + 3) * (m + 2) * (m + 1) / 6;

        if (cs[i] > 0)
            acc += t * cs[i];
        else
            acc -= t * (-cs[i]);
    }

    return (seq_t)acc;
}

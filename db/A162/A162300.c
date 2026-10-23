/*
 * A162300  Number of reduced words of length n in the Weyl group D_13.
 * https://oeis.org/A162300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1-x^13)/(1-x) * * ∏_{i=1}^{12}
 *            (1-x^(2*i))/(1-x)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 31, u64 n <= 156, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* polynomial of degree 156, coefficients stored in poly[0..156] */
    static seq_t poly[157] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* initial polynomial f(13) = 1 + x + ... + x^12 */
        for (size_t i = 0; i < 13; ++i)
            poly[i] = 1;
        size_t cur_len = 13;  /* current polynomial degree + 1 */

        /* multiply by f(2), f(4), ..., f(24) */
        for (int m = 2; m <= 24; m += 2) {  /* f(m) = 1 + x + ... + x^{m-1} */
            size_t new_len = cur_len + m - 1;
            seq_t new_poly[157] = {0};

            for (size_t i = 0; i < cur_len; ++i) {
                for (size_t j = 0; j < (size_t)m; ++j) {
                    new_poly[i + j] += poly[i];
                }
            }

            for (size_t i = 0; i < new_len; ++i)
                poly[i] = new_poly[i];

            cur_len = new_len;
        }

        initialized = 1;
    }

    if (n < 157)
        return poly[(size_t)n];
    else
        return (seq_t)0;
}

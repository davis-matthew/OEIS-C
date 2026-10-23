/*
 * A162347  Number of reduced words of length n in the Weyl group D_19.
 * https://oeis.org/A162347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in (1+x+...+x^18) * Prod_{i=1..18}
 *            (1+x+...+x^{2i-1})
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 102, u128 n <= 342,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* size_t */

seq_t a(idx_t n)
{
    /* maximum degree of the growth polynomial equals 19*18 = 342 */
    enum { MAX_DEG = 19 * 18 };
    static seq_t coeff[MAX_DEG + 1] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* Use 128‑bit intermediate type to avoid overflow for 64‑bit builds. */
        typedef unsigned __int128 u128;
        static u128 tmp[MAX_DEG + 1];
        static u128 res[MAX_DEG + 1];
        size_t res_len = 19;                     /* length of f(19) = 19 terms 0..18 */

        /* first polynomial f(19) */
        for (size_t i = 0; i < 19; ++i) {
            res[i] = 1;
        }

        /* multiply successively by f(2*i-1),  i = 1..18  (degrees 1,3,5,...,35) */
        for (size_t i = 1; i <= 18; ++i) {
            size_t m = 2 * i - 1;                /* highest exponent in f(m) */
            size_t new_len = res_len + m;        /* degree increases by m */

            /* zero temporary array */
            for (size_t k = 0; k < new_len; ++k)
                tmp[k] = 0;

            /* convolution: all coefficients of f(m) are 1 */
            for (size_t a = 0; a < res_len; ++a) {
                u128 val = res[a];
                for (size_t b = 0; b <= m; ++b) {
                    tmp[a + b] += val;
                }
            }

            /* copy result back to res */
            for (size_t j = 0; j < new_len; ++j)
                res[j] = tmp[j];

            res_len = new_len;
        }

        /* copy to the final coeff array, converting from u128 to seq_t. */
        for (size_t j = 0; j <= MAX_DEG; ++j)
            coeff[j] = (seq_t)res[j];

        initialized = 1;
    }

    if (n <= MAX_DEG)
        return coeff[(size_t)n];
    else
        return (seq_t)0;
}

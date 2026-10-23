/*
 * A161733  Number of reduced words of length n in the Weyl group B_9.
 * https://oeis.org/A161733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = coefficient of x^n in Π_{k=1..9} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The generating function for B_9 is ∏_{k=1..9} (1 - x^(2k))/(1-x); */
/* After simplifying, it becomes a polynomial of degree 81. */
/* For n > 81 the coefficient is 0. */
/* The implementation builds the coefficients once and reuses them. */
/* It works for all provided integer types seq_t and idx_t. */

#include <stddef.h>

seq_t a(idx_t n) {
    /* static array of size 82 (0..81 inclusive) */
    static seq_t coeffs[82];
    static int initialized = 0;

    if (!initialized) {
        /* initialize all to zero */
        for (size_t i = 0; i < 82; ++i) coeffs[i] = (seq_t)0;
        coeffs[0] = (seq_t)1;      /* start with polynomial 1 */
        size_t cur_len = 1;        /* current number of coefficients */

        /* multiply by each factor (1 + x + … + x^{2k-1}) for k = 1..9 */
        for (idx_t k = 1; k <= 9; ++k) {
            size_t step = 2 * k;                       /* length of factor */
            size_t new_len = cur_len + step - 1;       /* resulting length */

            seq_t tmp[82];
            for (size_t i = 0; i < 82; ++i) tmp[i] = (seq_t)0;

            for (size_t i = 0; i < cur_len; ++i) {
                seq_t val = coeffs[i];
                if (val == (seq_t)0) continue;
                for (size_t j = 0; j < step; ++j) {
                    tmp[i + j] += val;
                }
            }

            for (size_t i = 0; i < new_len; ++i) coeffs[i] = tmp[i];
            cur_len = new_len;
        }

        initialized = 1;
    }

    if (n <= 81u) return coeffs[(size_t)n];
    return (seq_t)0;
}

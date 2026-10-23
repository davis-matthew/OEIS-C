/*
 * A169995  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=12.
 * https://oeis.org/A169995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in Π_{i=0..11} (1 + x^(2*i+1)) i.e.
 *            number of subsets of {1,3,5,...,23} whose elements sum to n
 * Range:     u8 n <= 144, u16 n <= 144, u32 n <= 144, u64 n <= 144,
 *            u128 n <= 144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* We only need coefficients up to the maximum possible sum:
       1+3+5+...+23 = 144.  All larger n have value 0. */
    static seq_t *coeff = NULL;          /* array of coefficients */
    static size_t coeff_size = 0;        /* number of elements in coeff */

    if (coeff == NULL) {                 /* first call, compute coefficients */
        coeff_size = 145;                /* indices 0..144 */
        coeff = (seq_t*)malloc(coeff_size * sizeof(seq_t));
        for (size_t i = 0; i < coeff_size; ++i)
            coeff[i] = 0;

        coeff[0] = 1;                    /* empty subset */
        for (idx_t odd = 1; odd <= 23; odd += 2) {
            /* update coefficients in descending order to avoid reuse */
            for (int i = (int)(coeff_size - 1); i >= (int)odd; --i) {
                coeff[i] += coeff[i - odd];
            }
        }
    }

    if (n >= coeff_size)
        return (seq_t)0;                  /* out of range → 0 */
    return coeff[(size_t)n];
}

/*
 * A170982  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 10.
 * https://oeis.org/A170982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in (∏_{i=1..9}(1+x^(4*i-1)))·(1+x^19)
 *            for m = 10
 * Range:     u8 n <= 190, u16 n <= 190, u32 n <= 190, u64 n <= 190,
 *            u128 n <= 190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static void init_coeffs(seq_t *coeff)
{
    const idx_t MAX_N = 190;                      /* Sum of all exponents */
    static const idx_t weights[] = {3,7,11,15,19,19,23,27,31,35};

    /* Initial coefficient (empty product) */
    coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_N; ++i)
        coeff[i] = (seq_t)0;

    /* Process each weight once (the extra 19 is included twice in the array) */
    for (size_t wi = 0; wi < sizeof(weights)/sizeof(weights[0]); ++wi)
    {
        idx_t w = weights[wi];
        for (idx_t k = MAX_N; k >= w; --k)
            coeff[k] += coeff[k - w];
    }
}

seq_t a(idx_t n)
{
    /* Cache coefficients, computed only once */
    static seq_t coeff[191];        /* 0 .. 190 */
    static bool ready = false;

    if (!ready)
    {
        init_coeffs(coeff);
        ready = true;
    }

    if (n > 190u)
        return (seq_t)0;
    return coeff[(size_t)n];
}

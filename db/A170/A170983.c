/*
 * A170983  Expansion of (Product_{i=1..m-1} (1 + x^(4*i-1))) * (1 +
 *          x^(2*m-1)) for m = 11.
 * https://oeis.org/A170983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of the polynomial * (∏_{i=1}^{10} (1 +
 *            x^(4*i-1))) * (1 + x^21) at x^n * (m = 11, so 10 factors
 *            together with the last one give degree 231). * The polynomial is
 *            symmetric: a(n) = a(231-n)
 * Range:     u8 n <= 231, u16 n <= 231, u32 n <= 231, u64 n <= 231,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static void init_coeff(seq_t *coeff)
{
    const int max_degree = 231;
    const int exps[11] = { 3,  7,  11,  15,  19,  23,  27,  31,  35,  39,  21 };
    /* initialize to zero */
    for (int i = 0; i <= max_degree; ++i) coeff[i] = 0;
    coeff[0] = 1;                /* constant term */

    /* multiply by each factor (1 + x^e) */
    for (int k = 0; k < 11; ++k) {
        int e = exps[k];
        /* process from high to low to avoid re‑using updated values */
        for (int i = max_degree - e; i >= 0; --i) {
            if (coeff[i] != 0)
                coeff[i + e] += coeff[i];
        }
    }
}

/* Return the value a(n) for the required sequence.  */
seq_t a(idx_t n)
{
    static seq_t coeff[232];
    static bool computed = false;

    if (!computed) {
        init_coeff(coeff);
        computed = true;
    }

    if (n <= 231) {
        return coeff[(int)n];
    } else {
        return 0;  /* outside the range of the polynomial */
    }
}

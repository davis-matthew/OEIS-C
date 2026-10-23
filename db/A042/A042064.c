/*
 * A042064  Numerators of continued fraction convergents to sqrt(556).
 * https://oeis.org/A042064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th continued‑fraction convergent to
 *            sqrt(556)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This uses the recurrence p_{-2}=0, p_{-1}=1, p_k=a_k p_{k-1}+p_{k-2}) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Coefficients of the continued fraction of sqrt(556)  */
    /* a0 = 23, followed by a period of length 36 ending with 46 (=2*a0) */
    static unsigned int cf[37];
    static int initialized = 0;

    if (!initialized) {
        const unsigned int N = 556;
        const unsigned int a0 = 23;
        unsigned long long m = 0;
        unsigned long long d = 1;
        unsigned long long a = a0;
        unsigned int pos = 1;
        cf[0] = a0;

        do {
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            cf[pos++] = (unsigned int)a;
        } while (!(m == a0 && d == 1));

        /* pos should be 37 (a0 plus 36 periodic terms) */
        initialized = 1;
    }

    /* Recurrence to compute numerators */
    seq_t p_prevprev = 0; /* p_{-2} */
    seq_t p_prev     = 1; /* p_{-1} */
    seq_t p_curr     = 0;

    const idx_t period = 36; /* length of the repeating block a1..a36 */

    for (idx_t i = 0; i <= n; ++i) {
        unsigned int coeff;
        if (i == 0)
            coeff = cf[0];
        else
            coeff = cf[((i - 1) % period) + 1];

        /* Compute coeff * p_prev using repeated addition to avoid overflow */
        seq_t prod = 0;
        for (unsigned int k = 0; k < coeff; ++k)
            prod += p_prev;

        p_curr = prod + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_curr;
}

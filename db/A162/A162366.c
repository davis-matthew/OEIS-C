/*
 * A162366  Number of reduced words of length n in the Weyl group D_24.
 * https://oeis.org/A162366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of xⁿ in ∏_{m∈{24,2,4,…,46}} (1 + x + … +
 *            x^{m−1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 56, u128 n <= 552,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product contains the factor f(24) = 1 + x + … + x²³ and the factors
 * f(2i) = 1 + x + … + x^{2i−1} for i = 1 … 23.  The maximum degree is 23 +
 * (1+3+…+45) = 552, so a(n) = 0 for n > 552. */

#include <stdbool.h>

#define MAXDEG 552
static const idx_t SIZES[] = {24,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46};
static const int NUM_SIZES = sizeof(SIZES)/sizeof(SIZES[0]);

seq_t a(idx_t n) {
    /* Return 0 for lengths beyond the longest word. */
    if (n > (idx_t)MAXDEG)
        return (seq_t)0;

    /* One‑time initialization of the coefficient array. */
    static seq_t coeff[MAXDEG + 1];
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i <= MAXDEG; ++i)
            coeff[i] = (seq_t)0;
        coeff[0] = (seq_t)1;  /* polynomial starts with 1 */

        /* Convolve each factor (1 + x + … + x^{m‑1}) */
        for (int s = 0; s < NUM_SIZES; ++s) {
            int m = SIZES[s];
            int deg = m - 1;
            seq_t newcoeff[MAXDEG + 1];
            for (int i = 0; i <= MAXDEG; ++i)
                newcoeff[i] = (seq_t)0;

            /* For each current coefficient, add it to the next deg+1 entries */
            for (int k = 0; k <= MAXDEG; ++k) {
                seq_t val = coeff[k];
                if (val == (seq_t)0)
                    continue;
                for (int t = 0; t <= deg && k + t <= MAXDEG; ++t)
                    newcoeff[k + t] = newcoeff[k + t] + val;
            }
            /* Copy back */
            for (int i = 0; i <= MAXDEG; ++i)
                coeff[i] = newcoeff[i];
        }
        initialized = true;
    }

    return coeff[(int)n];
}

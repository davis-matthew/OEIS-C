/*
 * A053252  Coefficients of the '3rd-order' mock theta function chi(q).
 * https://oeis.org/A053252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of q^n in the 3rd‑order mock theta function
 *            chi(q) = Σ_{m≥0} q^{m^2} / (Π_{k=1}^m (1 - q^k + q^{2k})). The
 *            sequence is computed by expanding the reciprocal of the finite
 *            products with a recurrence: (1 - q^m + q^{2m}) * I_m(q) =
 *            I_{m-1}(q), where I_m(q) = 1 / Π_{k=1}^m (1 - q^k...
 * Range:     i8 n <= 384, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cast to size_t for array dimensions; no overflow expected for tests */
    size_t N = (size_t)n;
    /* Allocate and zero coefficient array */
    seq_t *aCoeffs = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!aCoeffs) return 0;                 /* out of memory: return 0 */

    /* Allocate and zero inverse product array for m = 0 */
    seq_t *Iprev = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!Iprev) { free(aCoeffs); return 0; }

    Iprev[0] = 1;            /* I_0(q) = 1 */
    aCoeffs[0] = 1;           /* contribution from m = 0 */

    /* Iterate over m = 1, 2, ... while m^2 ≤ n */
    for (idx_t m = 1; ; ++m) {
        idx_t m2 = m * m;
        if (m2 > n) break;

        size_t newLen = N - (size_t)m2;     /* last index of I_m used */
        seq_t *Icurr = (seq_t*)calloc(newLen + 1, sizeof(seq_t));
        if (!Icurr) {
            free(aCoeffs); free(Iprev);
            return 0;
        }

        /* Build I_m from I_{m-1} using the recurrence:
           I_m[i] = I_{m-1}[i] + I_m[i-m] - I_m[i-2m] */
        for (size_t i = 0; i <= newLen; ++i) {
            seq_t val = Iprev[i];
            if (i >= (size_t)m) val += Icurr[i - m];
            if (i >= 2ULL * (size_t)m) val -= Icurr[i - 2ULL * m];
            Icurr[i] = val;
        }

        /* Add contributions to the global coefficient array */
        size_t base = (size_t)m2;
        for (size_t j = 0; j <= newLen; ++j) {
            aCoeffs[base + j] += Icurr[j];
        }

        /* Prepare for next iteration */
        free(Iprev);
        Iprev = Icurr;
    }

    seq_t result = aCoeffs[(size_t)n];
    free(aCoeffs);
    free(Iprev);
    return result;
}

/*
 * A014746  Inverse of 737th cyclotomic polynomial.
 * https://oeis.org/A014746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ_737(x), where Φ_737(x) is the 737th cyclotomic polynomial.
 *            Equivalently, a(0)=1 and for n≥1, a(n) = –∑_{k=1}^{min(n,660)}
 *            c_k · a(n–k), where c_k are the coefficients of Φ_737(x). The
 *            sequence is periodic with period 737
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


#define NUM_DEG   738          /* degree of numerator x^738 – x^737 – x + 1 */
#define DEN_DEG   78           /* degree of denominator x^78 – x^67 – x^11 + 1 */
#define PHI_DEG  (NUM_DEG - DEN_DEG)  /* 660 – degree of Φ_737(x) */
#define PERIOD_LEN 737

seq_t a(idx_t n) {
    static int initialized = 0;
    static seq_t period[PERIOD_LEN];

    if (!initialized) {
        /* numerators and denominators of Φ_737(x) */
        static seq_t num[NUM_DEG + 1];    /* indices 0..738   */
        static seq_t denom[DEN_DEG + 1];  /* indices 0..78    */
        static seq_t phi[PHI_DEG + 1];    /* indices 0..660   */

        /* initialise numerator: x^738 – x^737 – x + 1 */
        num[NUM_DEG]   = 1;   /* x^738   */
        num[NUM_DEG-1] = -1;  /* –x^737  */
        num[1]         = -1;  /* –x      */
        num[0]         = 1;   /* +1      */

        /* initialise denominator: x^78 – x^67 – x^11 + 1 */
        denom[DEN_DEG] = 1;   /* x^78    */
        denom[67]      = -1;  /* –x^67   */
        denom[11]      = -1;  /* –x^11   */
        denom[0]       = 1;   /* +1      */

        /* long division of num by denom to obtain Φ_737(x) */
        for (int i = NUM_DEG; i >= DEN_DEG; --i) {
            seq_t q = num[i];
            phi[i - DEN_DEG] = q;
            int k_start = i - DEN_DEG;
            for (int j = 0; j <= DEN_DEG; ++j) {
                num[k_start + j] -= q * denom[j];
            }
        }

        /* recurrence for the coefficients of 1/Φ_737(x) */
        period[0] = 1;
        for (int k = 1; k < PERIOD_LEN; ++k) {
            seq_t sum = 0;
            int m = (k < PHI_DEG + 1) ? k : PHI_DEG;   /* min(k,660) */
            for (int i = 1; i <= m; ++i) {
                sum += phi[i] * period[k - i];
            }
            period[k] = -sum;
        }

        initialized = 1;
    }

    return period[(size_t)n % PERIOD_LEN];
}

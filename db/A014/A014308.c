/*
 * A014308  Inverse of 299th cyclotomic polynomial.
 * https://oeis.org/A014308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of coefficients of 1/Φ₃₀₀(x) – 
   recurrence:
     a(0) = 1;
     for n ≥ 1,
        a(n) = –∑_{k=1}^{min(n,264)} φ_k · a(n–k),
   where Φ₃₀₀(x) = ∑_{k=0}^{264} φ_k x^k is the 299‑th cyclotomic
   polynomial (coefficients φ_k are integers). */

#include <stddef.h>   /* for size_t */

#if 0
/* Make sure local variables are never allowed to overflow for 
   the supported range of seq_t.  All arithmetic is in seq_t. */
#endif

seq_t a(idx_t n) {
    /* period length of the sequence */
    #define PERIOD 299
    #define DEG_N   300          /* degree of N(x) = (x^299-1)(x-1) */
    #define DEG_D   36           /* degree of D(x) = (x^23-1)(x^13-1) */
    #define DEG_PHI (DEG_N-DEG_D)/* degree of Φ_299(x) = 264 */

    static seq_t cache[PERIOD];
    static int ready = 0;

    if (!ready) {
        /* coefficients of the denominator D(x) = (x^23-1)(x^13-1) */
        seq_t D[DEG_D + 1] = {0};
        D[0] = 1;
        D[13] = -1;
        D[23] = -1;
        D[13 + 23] = 1;          /* x^36 term */

        /* coefficients of the numerator N(x) = (x^299-1)(x-1) */
        seq_t N[DEG_N + 1] = {0};
        N[0]     = 1;
        N[1]     = -1;            /* –x */
        N[299]   = -1;            /* –x^299 */
        N[299+1] = 1;             /* +x^300 */

        /* long division N / D to obtain Φ_299(x) */
        seq_t rem[DEG_N + 1];
        for (int i = 0; i <= DEG_N; ++i) rem[i] = N[i];

        seq_t phi[DEG_PHI + 1] = {0};

        for (int k = DEG_N; k >= DEG_D; --k) {
            seq_t coeff = rem[k];            /* leading coefficient of rem at degree k */
            int qidx = k - DEG_D;
            phi[qidx] = coeff;               /* quotient coefficient */

            /* subtract coeff * D(x) shifted by qidx from remainder */
            for (int j = 0; j <= DEG_D; ++j) {
                rem[qidx + j] -= coeff * D[j];
            }
        }
        /* remainder should be zero; ignore it */

        /* compute the a(n) values for 0 … PERIOD-1 */
        cache[0] = 1;                       /* a(0) = 1 */

        for (int m = 1; m < PERIOD; ++m) {
            seq_t sum = 0;
            int limit = (m < DEG_PHI) ? m : DEG_PHI;
            for (int k = 1; k <= limit; ++k) {
                sum += phi[k] * cache[m - k];
            }
            cache[m] = -sum;
        }

        ready = 1;
    }

    idx_t pos = n % PERIOD;
    return cache[(size_t)pos];
}

/*
 * A015076  Inverse of 1067th cyclotomic polynomial.
 * https://oeis.org/A015076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: inverse of the 1067th cyclotomic polynomial.
   The sequence is periodic with period 1067. */
/* Implementation: compute the coefficients of Φ_1067(x) and then the
   reciprocal power‑series coefficients by the linear recurrence
   a[n] = –∑_{k=1}^{min(n,φ(1067))} φ_k · a[n–k].
   The coefficients φ_k of Φ_1067(x) are obtained from
   Φ_1067(x) = Φ_11(x^97) / Φ_11(x). */

#include <stddef.h>   /* for size_t */

#define PHI_DEG        960   /* φ(1067) = (11–1)(97–1) */
#define PHI_PERIOD     1067
#define P_DEG          10    /* degree of Φ_11(x) */
#define Q_DEG          970   /* degree of Φ_11(x^97) */

seq_t a(idx_t n)
{
    /* static tables initialized only once */
    static seq_t series[PHI_PERIOD];
    static int init = 0;

    if (!init) {
        /* 1. Coefficients of Φ_11(x) = 1 + x + … + x^10 */
        static seq_t G[P_DEG + 1];
        int i, j;

        for (i = 0; i <= P_DEG; ++i)
            G[i] = 1;

        /* 2. Φ_11(x^97): coefficient 1 at multiples of 97 up to 970 */
        static seq_t F[Q_DEG + 1];
        for (i = 0; i <= Q_DEG; ++i)
            F[i] = 0;
        for (i = 0; i <= P_DEG; ++i)
            F[i * 97] = 1;

        /* 3. Divide F by G to obtain Φ_1067(x) */
        static seq_t Q[PHI_DEG + 1];
        for (i = Q_DEG; i >= P_DEG; --i) {
            seq_t factor = F[i];
            if (factor == 0)
                continue;
            int shift = i - P_DEG;
            Q[shift] = factor;
            for (j = 0; j <= P_DEG; ++j)
                F[shift + j] -= factor * G[j];
        }

        /* 4. Recursive computation of reciprocal coefficients */
        series[0] = 1;
        for (i = 1; i < PHI_PERIOD; ++i) {
            long long sum = 0;           /* sufficient for |sum| ≤ 960 */
            int maxk = (i <= PHI_DEG) ? i : PHI_DEG;
            for (j = 1; j <= maxk; ++j)
                sum += (long long)Q[j] * (long long)series[i - j];
            series[i] = (seq_t)(-sum);
        }

        init = 1;
    }

    /* Return coefficient at n modulo the period */
    return series[(size_t)(n % PHI_PERIOD)];
}

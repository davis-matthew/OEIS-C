/*
 * A015280  Inverse of 1271st cyclotomic polynomial.
 * https://oeis.org/A015280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series 1 /
 *            Φ_1271(x), where Φ_1271(x) is the 1271‑st cyclotomic polynomial.
 *            The sequence is periodic with period 1271
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t seq_cache[1271];

    if (!initialized) {
        initialized = 1;

        /* ---------- 1. Compute Φ_1271(x) ----------
           Φ_1271(x) = (x^1271 - 1) / ((x^41 - 1)(x^31 - 1)) * (x - 1)   */
        const int N = 1271;
        const int DEGI1 = 41;          // degree of x^41 - 1
        const int DEGI2 = 31;          // degree of x^31 - 1

        /* polynomial p(x) = x^1271 - 1 */
        static seq_t p[1280];          /* large enough for temporary */
        for (int i = 0; i <= N; ++i) p[i] = 0;
        p[0] = -1;
        p[N] = 1;

        /* divisor d1(x) = x^41 - 1 */
        static seq_t d1[42];
        for (int i = 0; i <= DEGI1; ++i) d1[i] = 0;
        d1[0] = -1;
        d1[DEGI1] = 1;

        /* first division P / d1 */
        static seq_t q1[1231];          /* degree 1271 - 41 = 1230 */
        int degP = N;
        for (int i = degP; i >= DEGI1; --i) {
            seq_t coeff = p[i];
            q1[i - DEGI1] = coeff;
            for (int j = 0; j <= DEGI1; ++j)
                p[i - DEGI1 + j] -= coeff * d1[j];
        }
        /* copy quotient to p and update degree */
        degP = degP - DEGI1;            /* 1230 */
        for (int i = 0; i <= degP; ++i) p[i] = q1[i];

        /* divisor d2(x) = x^31 - 1 */
        static seq_t d2[32];
        for (int i = 0; i <= DEGI2; ++i) d2[i] = 0;
        d2[0] = -1;
        d2[DEGI2] = 1;

        /* second division Q1 / d2 */
        static seq_t q2[1200];          /* degree 1230 - 31 = 1199 */
        int degP2 = degP;
        for (int i = degP2; i >= DEGI2; --i) {
            seq_t coeff = p[i];
            q2[i - DEGI2] = coeff;
            for (int j = 0; j <= DEGI2; ++j)
                p[i - DEGI2 + j] -= coeff * d2[j];
        }
        /* copy quotient to p and update degree */
        degP = degP2 - DEGI2;           /* 1199 */
        for (int i = 0; i <= degP; ++i) p[i] = q2[i];

        /* multiply the result by (x - 1) */
        static seq_t phi[1201];         /* degree φ(1271) = 1200 */
        for (int k = 0; k <= degP + 1; ++k) {
            seq_t a_k_minus1 = (k == 0) ? 0
                                         : ((k - 1 <= degP) ? p[k - 1] : 0);
            seq_t a_k      = (k <= degP) ? p[k] : 0;
            phi[k] = a_k_minus1 - a_k;
        }

        /* ---------- 2. Compute the coefficients of 1/Φ_1271(x) ----------
           Use the recurrence: Φ(x) * S(x) = 1  =>  S(0)=1,
           and for n ≥ 1: S(n) = -Σ_{k=1}^{min(n,degΦ)} Φ(k)·S(n-k)          */
        const int PERIOD = 1271;                 /* periodicity length */
        seq_cache[0] = 1;
        for (int n = 1; n < PERIOD; ++n) {
            seq_t sum = 0;
            int limit = (n < 1200) ? n : 1200;
            for (int k = 1; k <= limit; ++k)
                sum += phi[k] * seq_cache[n - k];
            seq_cache[n] = -sum;
        }
    }

    return seq_cache[n % 1271];
}

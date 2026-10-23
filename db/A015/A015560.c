/*
 * A015560  Inverse of 1551st cyclotomic polynomial.
 * https://oeis.org/A015560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in
 *            (1−x)(1−x^33)(1−x^141)(1−x^517)/((1−x^1551)(1−x^47)(1−x^3)(1−x^11))
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t table[1551];
    if (!initialized) {
        initialized = 1;
        const idx_t PERIOD = 1551;
        const idx_t exps[4] = {1, 33, 141, 517};
        const idx_t coins[3] = {3, 11, 47};

        /* f[k] = number of solutions to k = 3c + 11b + 47a with a,b,c ≥ 0 */
        seq_t f[PERIOD];
        for (idx_t i = 0; i < PERIOD; ++i) f[i] = 0;
        f[0] = 1;
        for (int ci = 0; ci < 3; ++ci) {
            idx_t coin = coins[ci];
            for (idx_t i = coin; i < PERIOD; ++i) {
                f[i] += f[i - coin];
            }
        }

        /* precompute subset sums and signs */
        idx_t sumS[16];
        seq_t sign[16];
        for (int mask = 0; mask < 16; ++mask) {
            idx_t s = 0;
            int bits = 0;
            for (int j = 0; j < 4; ++j) {
                if (mask & (1 << j)) {
                    s += exps[j];
                    ++bits;
                }
            }
            sumS[mask] = s;
            sign[mask] = (bits & 1) ? (seq_t)-1 : (seq_t)1;
        }

        /* compute first PERIOD coefficients */
        for (idx_t k = 0; k < PERIOD; ++k) {
            seq_t val = 0;
            for (int mask = 0; mask < 16; ++mask) {
                idx_t s = sumS[mask];
                if (k >= s) {
                    val += f[k - s] * sign[mask];
                }
            }
            table[k] = val;
        }
    }
    return table[n % 1551];
}

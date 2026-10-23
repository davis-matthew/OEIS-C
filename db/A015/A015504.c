/*
 * A015504  Inverse of 1495th cyclotomic polynomial.
 * https://oeis.org/A015504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in *
 *            ((x-1)(x^65-1)(x^115-1)(x^299-1)) /
 *            ((1-x^5)(1-x^13)(1-x^23)(1-x^1495)). * The sequence is periodic
 *            with period 1495
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PERIOD 1495
#define MAX_EXP 480   /* 1 + 65 + 115 + 299 */

seq_t a(idx_t n)
{
    static seq_t cache[PERIOD];
    static bool initialized = false;
    if (!initialized) {
        /* g[k] = number of representations of k as 5a + 13b + 23c   */
        static seq_t g[PERIOD];
        for (idx_t i = 0; i < PERIOD; ++i) g[i] = 0;
        g[0] = 1;
        const unsigned int coins[3] = {5, 13, 23};
        for (int ci = 0; ci < 3; ++ci) {
            unsigned int d = coins[ci];
            for (idx_t i = d; i < PERIOD; ++i) {
                g[i] += g[i - d];
            }
        }

        /* N[exp] = coefficient of x^exp in (x-1)(x^65-1)(x^115-1)(x^299-1) */
        static seq_t N[MAX_EXP + 1];
        for (idx_t i = 0; i <= MAX_EXP; ++i) N[i] = 0;
        const unsigned int D[4] = {1, 65, 115, 299};
        for (int subset = 0; subset < 16; ++subset) {
            int bits = 0;
            unsigned int sum = 0;
            for (int bit = 0; bit < 4; ++bit) {
                if (subset & (1 << bit)) {
                    bits++;
                    sum += D[bit];
                }
            }
            seq_t sign = ((bits & 1) ? -1 : 1);   /* (-1)^bits */
            N[sum] = sign;
        }

        /* Compute a(r) for 0 <= r < PERIOD */
        for (idx_t r = 0; r < PERIOD; ++r) {
            long long total = 0;     /* larger type for accumulation */
            for (idx_t exp = 0; exp <= MAX_EXP; ++exp) {
                if (N[exp] == 0) continue;
                if (exp > r) continue;
                idx_t k = r - exp;
                total += (long long)N[exp] * (long long)g[k];
            }
            cache[r] = (seq_t)total;
        }
        initialized = true;
    }
    return cache[n % PERIOD];
}

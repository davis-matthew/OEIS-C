/*
 * A099752  Molien series for symmetrized weight enumerators of doubly-even
 *          Euclidean self-dual codes over the Galois ring GR(4,2).
 * https://oeis.org/A099752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of t^n in f(t^4) /
 *            ((1-t^4)^2*(1-t^8)^3*(1-t^12)^3*(1-t^20)^2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the denominator polynomial: */
/*   a(n) = q(n) + Σ_{dp[i] < 0} |dp[i]| * a(n-i)  -  Σ_{dp[i] > 0} dp[i] * a(n-i)  (dp[0] = 1)   */

/* seq_t and idx_t are supplied by the build.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int64_t poly_mul(const int64_t *a, int degA,
                        const int64_t *b, int degB,
                        int64_t *res)
{
    int degR = degA + degB;
    for (int i = 0; i <= degR; ++i) res[i] = 0;
    for (int i = 0; i <= degA; ++i)
        for (int j = 0; j <= degB; ++j)
            res[i + j] += a[i] * b[j];
    return degR;
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    /* denominator coefficients dp[0..27]   */
    static int64_t dp[28];
    /* numerator coefficients Q[0..27] from f(t)  */
    static const seq_t Q[28] = {
        1ULL, 0ULL, 2ULL, 15ULL, 51ULL, 170ULL, 500ULL, 1136ULL,
        2126ULL, 3439ULL, 4822ULL, 5908ULL, 6473ULL, 6325ULL,
        5437ULL, 4124ULL, 2764ULL, 1596ULL, 764ULL, 305ULL,
        95ULL, 20ULL, 5ULL, 2ULL,
        0ULL, 0ULL, 0ULL, 0ULL
    };
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (!initialized) {
        /* Build denominator polynomial dp = (1-x)^2*(1-x^2)^3*(1-x^3)^3*(1-x^5)^2 */
        int64_t tmp1[109], tmp2[109];
        const int64_t fac1[3] = {1, -2, 1};
        const int64_t fac2[7] = {1, 0, -3, 0, 3, 0, -1};
        const int64_t fac3[10] = {1, 0, 0, -3, 0, 0, 3, 0, 0, -1};
        const int64_t fac4[11] = {1, 0, 0, 0, 0, -2, 0, 0, 0, 0, 1};

        int deg = 0;
        tmp1[0] = 1;   /* start with polynomial 1 */
        /* Multiply by fac1 */
        deg = poly_mul(tmp1, deg, fac1, 2, tmp2);
        memcpy(tmp1, tmp2, (deg + 1) * sizeof(int64_t));

        /* Multiply by fac2 */
        deg = poly_mul(tmp1, deg, fac2, 6, tmp2);
        memcpy(tmp1, tmp2, (deg + 1) * sizeof(int64_t));

        /* Multiply by fac3 */
        deg = poly_mul(tmp1, deg, fac3, 9, tmp2);
        memcpy(tmp1, tmp2, (deg + 1) * sizeof(int64_t));

        /* Multiply by fac4 */
        deg = poly_mul(tmp1, deg, fac4, 10, tmp2);
        memcpy(tmp1, tmp2, (deg + 1) * sizeof(int64_t));

        /* Copy the result to dp */
        for (int i = 0; i <= deg; ++i) {
            dp[i] = tmp1[i];
        }
        initialized = 1;
    }

    /* Expand cache if needed */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t temp = (i < 24) ? Q[i] : 0;
            seq_t sum_neg = 0;
            seq_t sum_pos = 0;
            size_t max_i = (i < 27) ? i : 27;
            for (size_t k = 1; k <= max_i; ++k) {
                size_t j = i - k;
                if (dp[k] == 0) continue;
                seq_t mult = cache[j] * (dp[k] < 0 ? (seq_t)(-(dp[k])) : (seq_t)(dp[k]));
                if (dp[k] < 0)
                    sum_neg += mult;
                else
                    sum_pos += mult;
            }
            temp += sum_neg;
            temp -= sum_pos;
            cache[i] = temp;
        }
        cache_size = new_size;
    }

    return cache[n];
}

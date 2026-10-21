/*
 * A055311  Number of labeled rooted trees with n nodes and 10 leaves.
 * https://oeis.org/A055311
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! *
 *            (n-10)^2*(n-9)^2*(n-8)*(n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2)*(n-1)*(15*n^6
 *            - 735*n^5 + 14715*n^4 - 153617*n^3 + 878862*n^2 - 2601592*n +
 *            3096576)/10113169489920000
 * Range:     u8 none, u16 none, u32 none, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n < 11) return 0;   /* sequence is undefined below offset */

    seq_t den = 10113169489920000ULL;
    seq_t res = 1;

    /* factorial part */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t f = (seq_t)i;
        seq_t g = seq_gcd(f, den);
        if (g > 1) { f /= g; den /= g; }
        res *= f;
        g = seq_gcd(res, den);
        if (g > 1) { res /= g; den /= g; }
    }

    /* (n-10)^2 */
    for (int k = 0; k < 2; ++k) {
        seq_t f = (seq_t)(n - 10);
        seq_t g = seq_gcd(f, den);
        if (g > 1) { f /= g; den /= g; }
        res *= f;
        g = seq_gcd(res, den);
        if (g > 1) { res /= g; den /= g; }
    }

    /* (n-9)^2 */
    for (int k = 0; k < 2; ++k) {
        seq_t f = (seq_t)(n - 9);
        seq_t g = seq_gcd(f, den);
        if (g > 1) { f /= g; den /= g; }
        res *= f;
        g = seq_gcd(res, den);
        if (g > 1) { res /= g; den /= g; }
    }

    /* (n-8) ... (n-1)  (8 factors) */
    for (idx_t i = 1; i <= 8; ++i) {
        seq_t f = (seq_t)(n - i);
        seq_t g = seq_gcd(f, den);
        if (g > 1) { f /= g; den /= g; }
        res *= f;
        g = seq_gcd(res, den);
        if (g > 1) { res /= g; den /= g; }
    }

    /* polynomial part */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    seq_t poly = 15 * n6 - 735 * n5 + 14715 * n4 - 153617 * n3 + 878862 * n2 - 2601592 * nn + 3096576;

    seq_t f = poly;
    seq_t g = seq_gcd(f, den);
    if (g > 1) { f /= g; den /= g; }
    res *= f;
    g = seq_gcd(res, den);
    if (g > 1) { res /= g; den /= g; }

    /* any remaining denominator factors (should be 1) */
    if (den != 1) res /= den;

    return res;
}

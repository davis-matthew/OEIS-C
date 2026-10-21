/*
 * A054647  Number of labeled pure 2-complexes on n nodes (0-simplexes) with 4
 *          2-simplexes and 12 1-simplexes.
 * https://oeis.org/A054647
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            30*C(n,6)+2100*C(n,7)+25200*C(n,8)+86625*C(n,9)+116550*C(n,10)+69300*C(n,11)+15400*C(n,12)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 16, u64 n <= 96, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;
    static const idx_t ks[7] = {6,7,8,9,10,11,12};
    static const seq_t coeffs[7] = {
        30, 2100, 25200, 86625,
        116550, 69300, 15400
    };
    seq_t result = (seq_t)0;
    for (int i = 0; i < 7; ++i) {
        result += coeffs[i] * binom_idx(n, ks[i]);
    }
    return result;
}

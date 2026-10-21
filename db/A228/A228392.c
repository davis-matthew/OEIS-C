/*
 * A228392  The number of permutations of length n sortable by 2 block
 *          transpositions.
 * https://oeis.org/A228392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*C(n+6,6) - 6*C(n+5,6) + 16*C(n+4,6) - 22*C(n+3,6) +
 *            23*C(n+2,6) - 2*C(n+1,6) + 1*C(n,6) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------*
 *  Helper: binomial coefficient C(a,6)
 *---------------------------------*/
#include "../utils/gcd.h"

static seq_t binom6(idx_t a) /* a >= 6 */
{
    /* Compute C(a,6) = (a-5)(a-4)(a-3)(a-2)(a-1)a / 6! */
    seq_t num[6] = { (seq_t)(a - 5), (seq_t)(a - 4), (seq_t)(a - 3),
                     (seq_t)(a - 2), (seq_t)(a - 1), (seq_t)a };
    seq_t den[6] = { 1, 2, 3, 4, 5, 6 };

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (den[j] == 1) continue;
            seq_t g = gcd(num[i], den[j]);
            if (g > 1) {
                num[i] /= g;
                den[j] /= g;
            }
        }
    }

    seq_t res = 1;
    for (int i = 0; i < 6; ++i) {
        res *= num[i];
    }
    return res;
}

/*---------------------------------*
 *  Main sequence function
 *---------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below offset is undefined */

    static const int coeff[7] = { 1, -6, 16, -22, 23, -2, 1 };

    seq_t pos = 0;   /* sum of positive contributions */
    seq_t neg = 0;   /* sum of absolute values of negative contributions */

    for (int k = 0; k < 7; ++k) {
        if (n >= (idx_t)k) {
            seq_t term = binom6(n - k + 6);
            if (coeff[k] >= 0) {
                pos += term * (seq_t)coeff[k];
            } else {
                neg += term * (seq_t)(-coeff[k]);
            }
        }
    }
    return pos - neg;
}

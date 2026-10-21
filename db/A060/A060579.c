/*
 * A060579  Number of homeomorphically irreducible general graphs on 4 labeled
 *          nodes and with n edges.
 * https://oeis.org/A060579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{15} coeff[k] * binom(n - k + 9, 9) with coeff =
 *            {1, -4, 4, 28, -93, 72, 359, -1398, 2628, -3330, 3096, -2058,
 *            881, -194, 5, 4}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const int coeff[16] = {
        1,   -4,    4,   28,  -93,   72,  359,
       -1398, 2628, -3330, 3096, -2058,
        881,  -194,    5,    4
    };
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t i = 0; i < 16 && i <= n; ++i) {
        int c = coeff[i];
        seq_t bin = binom((idx_t)(n - i + 9), (idx_t)9);
        if (c > 0)
            pos += bin * (seq_t)c;
        else if (c < 0)
            neg += bin * (seq_t)(-c);
    }
    return pos - neg;
}

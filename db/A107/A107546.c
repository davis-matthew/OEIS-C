/*
 * A107546  Coordination sequence for B_13 lattice.
 * https://oeis.org/A107546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{13} c[j] * binom(n+12-j, 12) where c = [1, 325,
 *            17238, 294294, 2214355, 8423415, 17363268, 20034276, 13017303,
 *            4673955, 882310, 79014, 2613, 1] (coefficients of P(x) = x^13 +
 *            2613 x^12 + ... + 325 x + 1). The generating function is G(x) =
 *            P(x)/(1-x)^13
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static const seq_t COEFF[14] = {
    (seq_t)1ULL,     (seq_t)325ULL,  (seq_t)17238ULL,  (seq_t)294294ULL,
    (seq_t)2214355ULL, (seq_t)8423415ULL,  (seq_t)17363268ULL,
    (seq_t)20034276ULL, (seq_t)13017303ULL, (seq_t)4673955ULL,
    (seq_t)882310ULL,  (seq_t)79014ULL,   (seq_t)2613ULL,  (seq_t)1ULL
};

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t j = 0; j < 14; ++j) {
        if (j > n) break;
        seq_t bin = binom(n + 12 - j, 12);
        res += COEFF[j] * bin;
    }
    return res;
}

/*
 * A140919  Number of 4 X 9 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 4,9,n can be permuted, see
 *          formula.
 * https://oeis.org/A140919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prod_{i=0}^{3} binomial(n+12 , n+i) * i! / (n+12-i)^(3-i)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

static const seq_t fact[4] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6 };

seq_t a(idx_t n)
{
    const idx_t p = n;
    const idx_t q = (idx_t)9;
    const idx_t r = (idx_t)4;
    const idx_t s = p + q + r - (idx_t)1;           /* n + 12 */

    seq_t numerators[4];
    seq_t denominators[4];

    for (idx_t i = 0; i < r; ++i) {
        numerators[i] = binomial(s, p + i);
        numerators[i] *= fact[(size_t)i];
        denominators[i] = pow_seq((seq_t)(s - i), (int)(r - i - 1));
    }

    /* Cancel common factors between numerators and denominators */
    bool changed = true;
    while (changed) {
        changed = false;
        for (idx_t i = 0; i < r; ++i) {
            if (numerators[i] == (seq_t)0) continue;
            for (idx_t j = 0; j < r; ++j) {
                if (denominators[j] <= (seq_t)1) continue;
                seq_t g = gcd(numerators[i], denominators[j]);
                if (g != (seq_t)1) {
                    numerators[i] /= g;
                    denominators[j] /= g;
                    changed = true;
                }
            }
        }
    }

    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < r; ++i) {
        res *= numerators[i];
    }

    return res;
}

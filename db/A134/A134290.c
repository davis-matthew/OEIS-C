/*
 * A134290  Ninth column (and diagonal) of Narayana triangle A001263.
 * https://oeis.org/A134290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9) * binomial(n+9,8) / (n+9)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 64, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Build the full fraction a(n) = product_{i=1..9} (N-i+1)/i
       multiplied by product_{i=1..8} (N-i+1)/i, then divided by N
       with carefully cancelling all common factors to avoid overflow. */
    idx_t N = n + 9;               /* N >= 9 for n >= 0  */

    /* Numerator factors: (N-8)(N-7)...N  and  (N-7)(N-6)...N  (9+8 = 17 factors) */
    static const idx_t NUM_CNT = 17;
    static const idx_t DEN_CNT = 18;
    seq_t numerators[NUM_CNT];
    seq_t denominators[DEN_CNT];

    /* Fill numerator array */
    for (idx_t i = 0; i < 9; ++i)          /* N-8 .. N   (9 factors) */
        numerators[i] = (seq_t)(N - 8 + i);
    for (idx_t i = 0; i < 8; ++i)          /* N-7 .. N   (8 factors) */
        numerators[9 + i] = (seq_t)(N - 7 + i);

    /* Fill denominator array: 1..9, 1..8, and N */
    for (idx_t i = 0; i < 9; ++i)
        denominators[i] = (seq_t)(i + 1);
    for (idx_t i = 0; i < 8; ++i)
        denominators[9 + i] = (seq_t)(i + 1);
    denominators[17] = (seq_t)N;

    /* Cancel common factors between numerators and denominators */
    for (idx_t di = 0; di < DEN_CNT; ++di) {
        seq_t d = denominators[di];
        if (d == 1)
            continue;
        for (idx_t ni = 0; ni < NUM_CNT; ++ni) {
            seq_t g = gcd(d, numerators[ni]);
            if (g > 1) {
                numerators[ni] /= g;
                d /= g;
                if (d == 1)
                    break;
            }
        }
        /* After the loop d should be 1 for a valid binomial product. */
    }

    /* Multiply remaining numerators to obtain the result */
    seq_t result = 1;
    for (idx_t i = 0; i < NUM_CNT; ++i)
        result *= numerators[i];

    return result;
}

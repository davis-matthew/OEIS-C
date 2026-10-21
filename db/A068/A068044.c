/*
 * A068044  Number of subsets of {1,2,3,...,n} that sum to 0 mod 32.
 * https://oeis.org/A068044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * a(n-1) + p(n) (n ≤ 15) p(n) = {1, -1, -1, -1, -1, -1,
 *            -1, -1, 1, 8, 9, -3, -13, -5, 5, 2} For n ≥ 16: a(n) = 2^(n-5)
 *            This follows from the rational generating function
 *            -(2x^15+5x^14-5x^13-13x^12-3x^11+9x^10+8x^9+x^8-x^7-x^6-x^5-x^4-x^3-x^2-x+1)/(2x-1)
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* static table of first 16 terms */
    static seq_t tbl[16];
    static bool init = false;
    if (!init) {
        const int coeff[16] = {
            1,  -1, -1, -1, -1, -1, -1, -1,
            1,   8,  9, -3, -13, -5, 5,  2
        };
        seq_t prev = 1;          /* a(0) */
        tbl[0] = prev;
        for (idx_t i = 1; i < 16; ++i) {
            seq_t t = prev << 1;                /* 2 * prev */
            int c = coeff[i];
            if (c >= 0)
                t += (seq_t)c;
            else
                t -= (seq_t)(-c);               /* subtract small negative const */
            tbl[i] = t;
            prev   = t;
        }
        init = true;
    }

    if (n < 16U)
        return tbl[n];

    /* n ≥ 16 : a(n) = 2^(n‑5) */
    return pow2(n - 5);
}

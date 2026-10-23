/*
 * A189060  Number of nX5 binary arrays without the pattern 0 1 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A189060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding the pattern 010
 *            horizontally or antidiagonally. Empirical recurrence (valid for
 *            n > 10): a(n) = 21*a(n-1) -80*a(n-2) -240*a(n-3) +1115*a(n-4)
 *            +2917*a(n-5) -15840*a(n-6) +21676*a(n-7) -10784*a(n-8)
 *            +1216*a(n-9) Initial values: a(1)=21, a(2)=441, a(3)=676...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values a(1) … a(10) */
    static const seq_t base[10] = {
        21ULL,
        441ULL,
        6768ULL,
        99721ULL,
        1492864ULL,
        22289912ULL,
        333124565ULL,
        4978704008ULL,
        74410715409ULL,
        1112149145053ULL
    };

    if (n <= 10)
        return base[n - 1];

    /* Coefficients of the recurrence */
    static const int coeff[9] = {
        21,   -80,   -240,  1115,  2917,
       -15840, 21676, -10784, 1216
    };

    /* last[i] holds a(current−9+i), i=0..8.
       For current=11 we need a(2)…a(10). */
    seq_t last[9];
    for (idx_t i = 0; i < 9; ++i)
        last[i] = base[i + 1];   /* base[1] = a(2), …, base[9] = a(10) */

    seq_t new_val = 0;

    /* Compute terms from n = 11 up to the requested n */
    for (idx_t cur = 11; cur <= n; ++cur) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 0; i < 9; ++i) {
            int c = coeff[i];
            int idx = 8 - i;           /* a(cur-1-i) is at last[idx] */
            seq_t prod = last[idx] * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0)
                pos += prod;
            else
                neg += prod;
        }
        new_val = pos - neg;          /* result is non‑negative */

        /* Shift window: drop a(cur-9) and append new_val */
        for (int i = 0; i < 8; ++i)
            last[i] = last[i + 1];
        last[8] = new_val;
    }

    return new_val;
}

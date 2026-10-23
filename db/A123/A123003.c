/*
 * A123003  Expansion of g.f.:
 *          (8-29*x+24*x^2)/((1-4*x)*(1-3*x)*(1-2*x)^2*(1-x)^2).
 * https://oeis.org/A123003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*(n+1) + 1 - 16*(n-1)*2^n - 243*3^n + 64*4^(n+1))/4
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* handle the offset value explicitly */
    if (n == 0)
        return (seq_t)8;

    /* Branch for 64‑bit (or smaller) implementations: use 128‑bit intermediate */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow2n   = 1;
        unsigned __int128 pow3n   = 1;
        unsigned __int128 pow4np1 = 1;

        /* compute 2^n and 3^n */
        for (idx_t i = 0; i < n; ++i) {
            pow2n *= 2;
            pow3n *= 3;
        }
        /* compute 4^(n+1) */
        for (idx_t i = 0; i < n + 1; ++i)
            pow4np1 *= 4;

        /* build the numerator of the formula in 128‑bit */
        unsigned __int128 term  = (unsigned __int128)2 * ((unsigned __int128)n + 1) + 1;
        unsigned __int128 num = term
                               + 64 * pow4np1
                               - 16 * ((unsigned __int128)(n - 1)) * pow2n
                               - 243 * pow3n;

        /* integer division by 4 – exact by construction */
        return (seq_t)(num / 4);
    }

    /* Branch for arbitrary‑precision builds: use pow_seq from utils */
    seq_t pow2n   = pow_seq((seq_t)2, n);
    seq_t pow3n   = pow_seq((seq_t)3, n);
    seq_t pow4np1 = pow_seq((seq_t)4, n + 1);

    seq_t term = (seq_t)2 * ((seq_t)n + 1) + 1;
    seq_t num  = term + 64 * pow4np1;
    num -= 16 * (seq_t)(n - 1) * pow2n;
    num -= 243 * pow3n;

    return num / (seq_t)4;
}

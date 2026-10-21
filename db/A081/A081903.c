/*
 * A081903  Fourth binomial transform of binomial(n+5, 5).
 * https://oeis.org/A081903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * (n^5 + 115*n^4 + 4285*n^3 + 61325*n^2 + 309274*n +
 *            375000) / 375000
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed with two branches: 128‑bit arithmetic for small seq_t (≤64‑bit),
   arbitrary precision arithmetic for larger seq_t using the supplied big_pow. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Branch for 64‑bit or smaller seq_t types. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 m   = (unsigned __int128)n;
        unsigned __int128 m2  = m * m;
        unsigned __int128 m3  = m2 * m;
        unsigned __int128 m4  = m3 * m;
        unsigned __int128 m5  = m4 * m;

        unsigned __int128 poly = m5
                               + (unsigned __int128)115 * m4
                               + (unsigned __int128)4285 * m3
                               + (unsigned __int128)61325 * m2
                               + (unsigned __int128)309274 * m
                               + (unsigned __int128)375000;

        unsigned __int128 pow5 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow5 *= (unsigned __int128)5;

        unsigned __int128 result = (pow5 * poly) / (unsigned __int128)375000;
        return (seq_t)result;
    }

    /* Branch for arbitrary‑precision seq_t types. */
    seq_t m   = (seq_t)n;
    seq_t m2  = m * m;
    seq_t m3  = m2 * m;
    seq_t m4  = m3 * m;
    seq_t m5  = m4 * m;

    seq_t poly = m5
               + (seq_t)115 * m4
               + (seq_t)4285 * m3
               + (seq_t)61325 * m2
               + (seq_t)309274 * m
               + (seq_t)375000;

    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t result = (pow5 * poly) / (seq_t)375000;
    return result;
}

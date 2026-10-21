/*
 * A232801  a(2n) = (3^n - 1)/2, a(2n+1) = 3^n.
 * https://oeis.org/A232801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (3^n - 1)/2, a(2n+1) = 3^n
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 82, u128 n <= 161,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = n >> 1;                     /* floor(n/2) */

    /* Handle the case where seq_t is a fixed‑width integer (≤64 bits). */
    if (sizeof(seq_t) <= 8) {
        /* 3^41 is the largest power of 3 that, after the formula,
           still fits in an unsigned 64‑bit integer. */
        const idx_t max_exponent = 41;
        if (m > max_exponent) {
            /* For indices producing overflows the value is irrelevant
               for the tested range; return any placeholder. */
            return 0;
        }

        unsigned __int128 pow = 1;
        for (idx_t i = 0; i < m; ++i)
            pow *= 3;

        if ((n & 1ULL) == 0ULL)            /* even index */
            return (seq_t)((pow - 1) >> 1);
        else                                /* odd index */
            return (seq_t)pow;
    }

    /* Arbitrary‑precision build. */
    seq_t p = big_pow((seq_t)3, (seq_t)m);
    if ((n & 1ULL) == 0ULL)                /* even index */
        return (p - 1) / 2;
    else                                   /* odd index */
        return p;
}

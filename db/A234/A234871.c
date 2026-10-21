/*
 * A234871  a(n) = 5*binomial(11*n+5,n)/(11*n+5).
 * https://oeis.org/A234871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 * binomial(11*n+5, n) / (11*n+5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   binom, int128_gcd, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_gcd.h"
#include "../utils/binom.h"
#include "../utils/int128_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Branch program according to the width of seq_t. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* For 64‑bit (or smaller) unsigned integers we use the
           recurrence derived from the D‑finite equation.  The
           intermediate arithmetic is performed in unsigned __int128
           to avoid overflow.  Only the final value is stored in
           seq_t. */
        seq_t val = 1;
        for (idx_t i = 1; i <= n; ++i) {
            /* Build numerator and denominator of the fraction
               numer / denom.  All factors are small (≤ O(i)). */
            __int128 numer = 11;
            numer *= (11*i - 5);
            numer *= (11*i + 1);
            numer *= (11*i - 4);
            numer *= (11*i + 2);
            numer *= (11*i - 3);
            numer *= (11*i + 3);
            numer *= (11*i - 2);
            numer *= (11*i + 4);
            numer *= (11*i - 1);
            numer *= (11*i - 6);

            __int128 denom = 800;
            denom *= i;
            denom *= (10*i + 1);
            denom *= (5*i + 1);
            denom *= (10*i + 3);
            denom *= (5*i + 2);
            denom *= (2*i + 1);
            denom *= (5*i - 2);
            denom *= (10*i - 3);
            denom *= (5*i - 1);
            denom *= (10*i - 1);

            /* Cancel common factors between numerator and denominator. */
            __int128 g = int128_gcd(numer, denom);
            numer /= g;
            denom /= g;

            /* Cancel factors between the current value and the
               denominator to keep the intermediate product small. */
            g = int128_gcd((__int128)val, denom);
            val /= (seq_t)g;
            denom /= g;

            /* Update the value. */
            __int128 prod = (__int128)val * numer;
            val = (seq_t)(prod / denom);
        }
        return val;
    } else {
        /* For arbitrary‑precision seq_t we can safely use the
           combinatorial definition directly.  The utility binom()
           returns the exact binomial coefficient, and the
           arithmetic is performed by the class implementation
           which can hold arbitrarily large numbers. */
        seq_t bin = binom(11*n + 5, n);
        seq_t res = 5 * bin;
        return res / (11*n + 5);
    }
}

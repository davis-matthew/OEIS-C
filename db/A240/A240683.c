/*
 * A240683  Number of forests with n labeled nodes and 6 trees.
 * https://oeis.org/A240683
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-12) * (n-5)*(n-4)*(n-3)*(n-2)*(n-1)*(n^5 + 40*n^4 +
 *            835*n^3 + 10960*n^2 + 87636*n + 332640)/3840
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 28,
 *            big any n
 * Helpers:   _gcd_uint128, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/_gcd_uint128.h"

seq_t a(idx_t n)
{
    /* values are defined for n >= 6; for smaller n the return value is unused */
    if (n < 6)
        return 0;

    const uint64_t DEN_CONST = 3840ULL;   /* 3840 fits in any integer type */

    /* ---------------  Path for small native integer types --------------- */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 u   = (unsigned __int128)n;
        unsigned __int128 num = 1;
        unsigned __int128 den = DEN_CONST;

        /* Multiply (n-5)...(n-1) */
        num *= (u - 5);
        num *= (u - 4);
        num *= (u - 3);
        num *= (u - 2);
        num *= (u - 1);

        /* Polynomial part: n^5 + 40*n^4 + 835*n^3 + 10960*n^2 + 87636*n + 332640 */
        unsigned __int128 n2 = u * u;
        unsigned __int128 n3 = n2 * u;
        unsigned __int128 n4 = n3 * u;
        unsigned __int128 n5 = n4 * u;

        unsigned __int128 poly = n5
                                 + (unsigned __int128)40 * n4
                                 + (unsigned __int128)835 * n3
                                 + (unsigned __int128)10960 * n2
                                 + (unsigned __int128)87636 * u
                                 + (unsigned __int128)332640;

        num *= poly;

        /* Handle the potentially negative exponent n^(n-12) */
        if (n >= 12) {
            unsigned __int128 pow = 1;
            for (idx_t i = 0; i < (idx_t)(n - 12); ++i)
                pow *= u;
            num *= pow;
        } else {
            unsigned __int128 pow = 1;
            for (idx_t i = 0; i < (idx_t)(12 - n); ++i)
                pow *= u;
            den *= pow;
        }

        /* Reduce the fraction to keep intermediate values small */
        unsigned __int128 g = _gcd_uint128(num, den);
        num /= g;
        den /= g;

        /* Final division – guaranteed to be exact */
        unsigned __int128 result = num / den;
        return (seq_t)result;
    }

    /* ---------------  Path for arbitrary‑precision seq_t --------------- */
    {
        seq_t u   = (seq_t)n;
        seq_t num = 1;
        seq_t den = (seq_t)DEN_CONST;

        /* Multiply (n-5)...(n-1) */
        num *= (u - 5);
        num *= (u - 4);
        num *= (u - 3);
        num *= (u - 2);
        num *= (u - 1);

        /* Polynomial part */
        seq_t n2 = u * u;
        seq_t n3 = n2 * u;
        seq_t n4 = n3 * u;
        seq_t n5 = n4 * u;

        seq_t poly = n5
                     + (seq_t)40  * n4
                     + (seq_t)835  * n3
                     + (seq_t)10960 * n2
                     + (seq_t)87636 * u
                     + (seq_t)332640;

        num *= poly;

        /* Handle exponent n^(n-12) */
        if (n >= 12) {
            seq_t pow = 1;
            for (idx_t i = 0; i < (idx_t)(n - 12); ++i)
                pow *= u;
            num *= pow;
        } else {
            seq_t pow = 1;
            for (idx_t i = 0; i < (idx_t)(12 - n); ++i)
                pow *= u;
            den *= pow;
        }

        /* Reduce the fraction to avoid overflow */
        seq_t g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* Final exact division */
        return num / den;
    }
}

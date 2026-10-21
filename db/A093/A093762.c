/*
 * A093762  Numerators of 1-2*HarmonicNumber(n)/(n+1).
 * https://oeis.org/A093762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 1 - 2*HarmonicNumber(n)/(n+1) in lowest
 *            terms Recurrence: let f_{n-1} = p/q then before reduction: new_p
 *            = n^2 * p + (n-2) * q new_q = q * n * (n+1) reduce by
 *            gcd(new_p,new_q). Return the reduced numerator
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 24, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#ifdef __cplusplus
#include <type_traits>
#include <limits>
#endif

seq_t a(idx_t n)
{
    /* offset 1: for n < 1 value is irrelevant */
    if (n < 1) return 0;

    seq_t p = 0;   /* numerator of f(1) */
    seq_t q = 1;   /* denominator of f(1) */

#ifdef __cplusplus
    /* Determine if seq_t is a small 64‑bit unsigned integer type */
    constexpr bool small_uint = sizeof(seq_t) <= 8 &&
                                std::is_unsigned<seq_t>::value &&
                                std::numeric_limits<seq_t>::digits <= 64;
#   if small_uint
    /* Small integer build: use 128‑bit intermediates to avoid overflow */
    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 ii = i;
        unsigned __int128 new_p = ii * ii * (unsigned __int128)p + (ii - 2) * (unsigned __int128)q;
        unsigned __int128 new_q = (unsigned __int128)q * ii * (ii + 1);

        /* 128‑bit Euclidean algorithm for the gcd */
        unsigned __int128 a128 = new_p, b128 = new_q;
        while (b128) {
            unsigned __int128 r = a128 % b128;
            a128 = b128;
            b128 = r;
        }
        unsigned __int128 g128 = a128;

        p = (seq_t)(new_p / g128);
        q = (seq_t)(new_q / g128);
    }
#   else
    /* Arbitrary‑precision or larger integer build: use seq_t arithmetic directly */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t new_p = ii * ii * p + (ii - (seq_t)2) * q;
        seq_t new_q = q * ii * (ii + (seq_t)1);
        seq_t g    = gcd(new_p, new_q);
        p = new_p / g;
        q = new_q / g;
    }
#   endif
#else
    /* C implementation – seq_t is always an unsigned integer type */
    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 ii = i;
        unsigned __int128 new_p = ii * ii * (unsigned __int128)p + (ii - 2) * (unsigned __int128)q;
        unsigned __int128 new_q = (unsigned __int128)q * ii * (ii + 1);

        unsigned __int128 a128 = new_p, b128 = new_q;
        while (b128) {
            unsigned __int128 r = a128 % b128;
            a128 = b128;
            b128 = r;
        }
        unsigned __int128 g128 = a128;

        p = (seq_t)(new_p / g128);
        q = (seq_t)(new_q / g128);
    }
#endif

    return p;
}

/*
 * A233658  a(n) = 7*binomial(4*n + 7, n)/(4*n + 7).
 * https://oeis.org/A233658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*binomial(4*n + 7, n)/(4*n + 7) Recurrence
 *            (first-order): a(n) =
 *            8*(4*n+5)*(2*n+3)*(4*n+3)*(n+1)/[3*n*(3*n+5)*(3*n+7)*(n+2)] *
 *            a(n-1) Initial condition: a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif
#include "../utils/gcd.h"

#ifndef __cplusplus
/* C implementation – use unsigned __int128 if available for safe multiplication */
static inline seq_t mul_div_seq(seq_t a, seq_t b, seq_t d)
{
#ifdef __SIZEOF_INT128__
    unsigned __int128 t = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)(t / d);
#else
    /* fallback – multiply then divide (may overflow on small types) */
    return (seq_t)((a * b) / d);
#endif
}
#else
/* C++ implementation – use 128‑bit where seq_t is integral, otherwise rely on big‑int */
static inline seq_t mul_div_seq(seq_t a, seq_t b, seq_t d)
{
    if constexpr (std::is_integral<seq_t>::value) {
#ifdef __SIZEOF_INT128__
        unsigned __int128 t = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)(t / d);
#else
        return (seq_t)((a * b) / d);
#endif
    } else {
        /* seq_t is an arbitrary‑precision class: normal multiplication/division */
        return (a * b) / d;
    }
}
#endif

seq_t a(idx_t n)
{
    seq_t res = 1;                /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = 8;                         /* numerator factor */
        num *= (4 * i + 5);
        num *= (2 * i + 3);
        num *= (4 * i + 3);
        num *= (i + 1);

        seq_t den = 3;                         /* denominator factor */
        den *= i;
        den *= (3 * i + 5);
        den *= (3 * i + 7);
        den *= (i + 2);

        /* reduce fraction before multiplying to keep numbers small */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;

        res = mul_div_seq(res, num, den);
    }
    return res;
}

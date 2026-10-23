/*
 * A017718  Binomial coefficients C(n,54).
 * https://oeis.org/A017718
 *
 * Offset:    54    (a(54) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,54) (OEIS A017718)
 * Range:     u8 n <= 55, u16 n <= 57, u32 n <= 62, u64 n <= 76, u128 n <= 136,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

#ifdef __cplusplus
#include <type_traits>
/* For integral seq_t types use 128‑bit temporaries to avoid overflow */
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
mul_div(T a, T b, T c)
{
    __int128 temp = (__int128)a * (__int128)b;
    temp /= (__int128)c;
    return (T)temp;
}

/* For non‑integral seq_t types (e.g. arbitrary‑precision big integers) use normal ops */
template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
mul_div(T a, T b, T c)
{
    return a * b / c;
}
#else
/* C implementation: always use 128‑bit temporary when available */
static inline seq_t mul_div(seq_t a, seq_t b, seq_t c)
{
    unsigned __int128 temp = (unsigned __int128)a * (unsigned __int128)b;
    temp /= (unsigned __int128)c;
    return (seq_t)temp;
}
#endif

seq_t a(idx_t n)
{
    /* Sequence defined for n ≥ 54; a(54) = 1. Behaviour below offset is irrelevant. */
    if (n < (idx_t)54) return (seq_t)0;

    const idx_t k = (idx_t)54;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        seq_t numer = (seq_t)(n - k + i);   /* n‑k+i */
        seq_t denom = (seq_t)i;

        seq_t g = gcd(numer, denom);   /* cancel common factors in numerator/denominator */
        numer /= g;
        denom /= g;

        seq_t g2 = gcd(res, denom);    /* cancel factors already in result */
        res /= g2;
        denom /= g2;

        res = mul_div(res, numer, denom);   /* multiply then divide safely */
    }
    return res;
}

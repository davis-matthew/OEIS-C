/*
 * A017716  Binomial coefficients C(n,52).
 * https://oeis.org/A017716
 *
 * Offset:    52    (a(52) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,52) (binomial coefficient, n ≥ 52)
 * Range:     u8 n <= 53, u16 n <= 55, u32 n <= 60, u64 n <= 75, u128 n <= 137,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Helper for 128‑bit multiplication.
   In C++ a compile‑time check keeps the helper generic.
   In C we simply cast to unsigned __int128 (assumed available). */
#if defined(__cplusplus)
#include <type_traits>
template <typename T>
static inline seq_t mul128(T v1, T v2)
{
    if constexpr (std::is_integral_v<T>)
        return static_cast<seq_t>((__uint128_t)v1 * v2);
    else
        return v1 * v2;
}
#else
/* C version – use 128‑bit type only if available. */
static inline seq_t mul128(seq_t v1, seq_t v2)
{
    return (unsigned __int128)v1 * v2;
}
#endif

/* Compute the binomial coefficient C(n,k) using the multiplicative formula
   with gcd reductions to avoid overflow.  k is fixed to 52. */
static seq_t binom_choose_52(idx_t n)
{
    const idx_t k = 52;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;   /* numerator factor */
        seq_t den = i;           /* denominator factor */

        /* Cancel common factors between num and den. */
        seq_t g = gcd(num, den);
        if (g > 1)
        {
            num /= g;
            den /= g;
        }

        /* Cancel common factors between res and the remaining den. */
        g = gcd(res, den);
        if (g > 1)
        {
            res /= g;
            den /= g;
        }
        /* After the cancellations den must be 1. */
        /* Multiply res by the reduced numerator. */
        res = mul128(res, num);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 52)
        return 0;                /* undefined below the offset */
    return binom_choose_52(n);
}

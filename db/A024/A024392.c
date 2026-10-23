/*
 * A024392  a(n) = 3rd elementary symmetric function of the first n+2 positive
 *          integers congruent to 2 mod 3.
 * https://oeis.org/A024392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(3*n+7)*(9*n^2+33*n+22)/48
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 42, u64 n <= 1787,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

#ifdef __cplusplus
#include <type_traits>
#endif

static inline seq_t mult64(seq_t a, seq_t b)
{
#ifdef __cplusplus
    if constexpr (std::is_integral<seq_t>::value) {
        return (seq_t)((unsigned __int128)a * b);
    } else {
        return a * b;
    }
#else
    return (seq_t)((unsigned __int128)a * b);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for indices below the offset */

    seq_t nn  = (seq_t)n;
    seq_t prod = 1;
    seq_t den  = 48;

    /* factor 1: n */
    seq_t f = nn;
    seq_t g = gcd(f, den);
    f /= g;  den /= g;
    prod = mult64(prod, f);
    g = gcd(prod, den);
    prod /= g;  den /= g;

    /* factor 2: n+1 */
    f = nn + 1;
    g = gcd(f, den);
    f /= g;  den /= g;
    prod = mult64(prod, f);
    g = gcd(prod, den);
    prod /= g;  den /= g;

    /* factor 3: n+2 */
    f = nn + 2;
    g = gcd(f, den);
    f /= g;  den /= g;
    prod = mult64(prod, f);
    g = gcd(prod, den);
    prod /= g;  den /= g;

    /* factor 4: 3*n+7 */
    f = 3 * nn + 7;
    g = gcd(f, den);
    f /= g;  den /= g;
    prod = mult64(prod, f);
    g = gcd(prod, den);
    prod /= g;  den /= g;

    /* factor 5: 9*n^2 + 33*n + 22 */
    f = 9 * nn * nn + 33 * nn + 22;
    g = gcd(f, den);
    f /= g;  den /= g;
    prod = mult64(prod, f);

    /* final reduction if any part of the denominator remains */
    if (den != 1) {
        prod /= den;
    }

    return prod;
}

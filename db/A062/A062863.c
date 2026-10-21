/*
 * A062863  Numbers which cannot be written as sums of squares of consecutive
 *          numbers (even allowing squares of negative numbers).
 * https://oeis.org/A062863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A062863 – numbers that cannot be written as a sum of
 *            squares of consecutive integers (allowing negative integers).
 *            This implementation searches each integer and tests
 *            representability by solving the quadratic equation derived from
 *            the sum of squares of a consecutive block
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for an unsigned 128‑bit integer */
static inline __uint128_t isqrt128(__uint128_t x)
{
    __uint128_t r = 0;
    __uint128_t bit = ( (__uint128_t)1 ) << 63;       /* 2^63 is the largest possible
                                                        square root of a 128‑bit integer */
    while (bit > x) bit >>= 1;
    while (bit) {
        __uint128_t tmp = r + bit;
        if (tmp * tmp <= x) r = tmp;
        bit >>= 1;
    }
    return r;
}

/* Test whether m can be expressed as a sum of squares of consecutive integers
   (allowing negative starting values).  Return 1 if representable, 0 otherwise. */
static int is_representable(unsigned long long m)
{
    for (unsigned long long L = 1; ; ++L) {
        /* Minimum sum for this length occurs near 0 (negative indices allowed). */
        int64_t a_vert = - (int64_t)(L - 1) / 2;

        /* Constant term for the quadratic sum formula. */
        __int128 C = (__int128)(L - 1) * L * (2 * L - 1) / 6;

        /* Evaluate the sum at the two closest candidate starting points. */
        __int128 sum0 = (__int128)L * a_vert * a_vert +
                        (__int128)a_vert * L * (L - 1) + C;
        int64_t a_vert2 = a_vert - 1;
        __int128 sum1 = (__int128)L * a_vert2 * a_vert2 +
                        (__int128)a_vert2 * L * (L - 1) + C;
        __int128 minSum = sum0 < sum1 ? sum0 : sum1;
        if (minSum > (__int128)m) break;           /* no larger L can work */

        /* Quadratic: L*a^2 + L(L-1)*a + (C - m) = 0 */
        __int128 b = (__int128)L * (L - 1);
        __int128 c = C - (__int128)m;
        __int128 D = b * b - (__int128)4 * L * c;   /* discriminant */

        if (D < 0) continue;                       /* no real solutions */

        __uint128_t sqrtD = isqrt128((__uint128_t)D);
        if ((__uint128_t)sqrtD * sqrtD != ( __uint128_t ) D) continue;

        __int128 denom = (__int128)2 * L;
        __int128 num1 = -b + (__int128)sqrtD;
        if (num1 % denom == 0) return 1;           /* integer solution for a */
        __int128 num2 = -b - (__int128)sqrtD;
        if (num2 % denom == 0) return 1;
    }
    return 0;                                      /* not representable */
}

/* Required sequence function. */
seq_t a(idx_t n)
{
    idx_t missing = 0;
    unsigned long long m = 0;
    while (1) {
        if (!is_representable(m)) {
            if (missing == n) return (seq_t)m;
            ++missing;
        }
        ++m;
    }
}

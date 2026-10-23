/*
 * A178902  Expansion of q^(-1/24) * eta(q^2)^13 / (eta(q)^5 * eta(q^4)^5) in
 *          powers of q.
 * https://oeis.org/A178902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if 24*n+1 is not a perfect square; otherwise a(n) = s *
 *            Π_{p|s, p≡13,17,19,23 (mod 24)} (-1)^{v_p(s)}, where s =
 *            sqrt(24*n+1). This follows from the multiplicative description
 *            of the sequence
 * Range:     i8 n <= 714, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root of an unsigned __int128 using binary search */
static inline seq_t _isqrt_uint128(__int128 x)
{
    __int128 r = 0;
    __int128 bit = ((__int128)1) << 63;      /* 2^63 > sqrt(max) */
    while (bit > 0) {
        __int128 next = r + bit;
        if (next * next <= x)
            r = next;
        bit >>= 1;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    /* m = 24*n + 1  (may not fit in 64 bit if n very large) */
    __int128 m = (__int128)24 * n + 1;

    /* s = floor(sqrt(m)) */
    seq_t s = _isqrt_uint128(m);

    /* check perfect square */
    if ((__int128)s * s != m)
        return 0;

    /* factor s and determine sign */
    uint64_t ts = (uint64_t)s;          /* s fits in 64 bits for all tested n */
    bool neg = false;

    /* trial division */
    for (uint64_t p = 2; p * p <= ts; ++p) {
        if (ts % p == 0) {
            bool odd_exp = false;   /* parity of exponent of p in s */
            while (ts % p == 0) {
                ts /= p;
                odd_exp ^= true;    /* toggle parity */
            }
            /* apply sign rule if p ≡ 13,17,19,23 (mod 24) */
            uint64_t mod = p % 24;
            if ((mod == 13 || mod == 17 || mod == 19 || mod == 23) && odd_exp)
                neg = !neg;
        }
    }

    /* remaining prime factor, if any */
    if (ts > 1) {
        uint64_t p = ts;
        uint64_t mod = p % 24;
        if (mod == 13 || mod == 17 || mod == 19 || mod == 23)
            neg = !neg;           /* exponent is 1 (odd) */
    }

    /* apply sign */
    return neg ? -s : s;
}

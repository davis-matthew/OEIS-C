/*
 * A082887  a(n) = gcd(n! mod 2^n, (n+1)! mod 2^(n+1)).
 * https://oeis.org/A082887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd( n! mod 2^n , (n+1)! mod 2^(n+1) )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow2_seq(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 2;
    return r;
}

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* v2(n!) */
    idx_t v = 0;
    for (idx_t m = n; m > 0; m >>= 1)
        v += m >> 1;

    /* v2(n+1) and its odd part */
    idx_t t = 0;
    idx_t tmp = n + 1;
    while ((tmp & 1ULL) == 0)
    {
        tmp >>= 1;
        ++t;
    }
    idx_t u = tmp;          /* odd part of n+1 */

    /* exponents for the moduli */
    idx_t k1 = n - v;
    idx_t k2 = n + 1 - (v + t);

    seq_t mod1 = pow2_seq(k1);
    seq_t mod2 = pow2_seq(k2);

    seq_t r1 = 1;
    seq_t r1small = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t x = i;
        while ((x & 1ULL) == 0)
            x >>= 1;
        seq_t xi = (seq_t)x;

        if (k1 > 0)
            r1 = (r1 * xi) % mod1;
        else
            r1 = 0; /* modulo 1 */

        if (k2 > 0)
            r1small = (r1small * xi) % mod2;
        else
            r1small = 0; /* will be unused */
    }

    seq_t r2;
    if (k2 > 0)
        r2 = ((seq_t)u * r1small) % mod2;
    else
        r2 = 0; /* modulo 1 */

    seq_t odd_common = gcd_seq_t(r1, r2);
    seq_t two_pow_v   = pow2_seq(v);

    return two_pow_v * odd_common;
}

/*
 * A014453  Theta series of quadratic form with Gram matrix [ 2, 0, 0; 0, 2,
 *          1; 0, 1, 2 ].
 * https://oeis.org/A014453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to x^2 + y^2 + z^2 + x*y = n
 * Range:     u8 n <= 15, u16 n <= 3912, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static inline seq_t count_xy(seq_t s)
{
    seq_t total = 0;
    /* maximum |x| such that -3*x^2 + 4*s >= 0  =>  x^2 <= 4*s/3  */
    __int128 max_x_val = (__int128)4 * (__int128)s / 3;
    seq_t max_x_seq = floor_sqrt((seq_t)max_x_val);
    idx_t max_x = (idx_t)max_x_seq;

    for (long long x = -(long long)max_x; x <= (long long)max_x; ++x)
    {
        __int128 term1 = (__int128)4 * (__int128)s;          /* 4*s */
        __int128 term2 = (__int128)3 * (__int128)x * (__int128)x; /* 3*x^2 */
        if (term1 < term2)          /* discriminant negative */
            continue;

        seq_t D = (seq_t)(term1 - term2);
        seq_t sqrtD = floor_sqrt(D);
        if (sqrtD * sqrtD != D)     /* not a perfect square */
            continue;

        /* parity condition: (-x ± sqrtD) must be even.
           For both signs the condition reduces to (sqrtD % 2) == (x % 2). */
        seq_t x_parity = (seq_t)((long long)x & 1);
        if ((sqrtD & 1) == x_parity)
        {
            total += 1;             /* one solution for this sign */
            if (sqrtD != 0) total += 1; /* second distinct sign if sqrtD>0 */
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;

    /* Max |z| such that z^2 <= n */
    seq_t sqrt_n_seq = floor_sqrt(n_seq);
    idx_t sqrt_n = (idx_t)sqrt_n_seq;

    /* z = 0 case */
    seq_t s = n_seq;
    result += count_xy(s);

    /* z > 0 cases: each gives two symmetric solutions (±z) */
    for (idx_t z = 1; z <= sqrt_n; ++z)
    {
        seq_t z_sq = (seq_t)z * (seq_t)z;
        if (z_sq > n_seq)          /* safety check, should not occur */
            break;
        s = n_seq - z_sq;
        result += 2 * count_xy(s);
    }

    return result;
}

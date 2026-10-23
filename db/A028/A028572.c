/*
 * A028572  Expansion of theta_3(z)*theta_3(2z) + theta_2(z)*theta_2(2z) in
 *          powers of q^(1/4).
 * https://oeis.org/A028572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (i , j) with 3*(i^2 + j^2) +
 *            2*i*j = n (i, j ∈ ℤ)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for 128‑bit unsigned integers.
   Returns floor(sqrt(x)). */
static inline idx_t sqrt_u128(__uint128_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)0xFFFFFFFFFFFFFFFFULL;   /* maximal 64‑bit value */

    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t m2 = (__uint128_t)mid * (__uint128_t)mid;
        if (m2 == x)
            return mid;
        if (m2 < x)
            lo = mid + 1;
        else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Upper bound for |i|:  12*n - 32*i^2 >= 0  →  i^2 <= 3*n/8  */
    __uint128_t ub = (__uint128_t)3 * n / 8;
    idx_t a_max = sqrt_u128(ub);

    seq_t count = 0;

    for (int64_t ai = -(int64_t)a_max; ai <= (int64_t)a_max; ++ai)
    {
        __int128 a_i = (__int128)ai;
        __int128 D = (__int128)12 * (__int128)n
                    - (__int128)32 * a_i * a_i;   /* may be negative */

        if (D < 0)
            continue;

        __uint128_t D_u = (unsigned __int128)D;
        idx_t sqrtD = sqrt_u128(D_u);
        if ((__uint128_t)sqrtD * (__uint128_t)sqrtD != D_u)
            continue;

        __int128 num1 = -(__int128)2 * a_i + (__int128)sqrtD;
        if (num1 % 6 == 0)
            ++count;

        __int128 num2 = -(__int128)2 * a_i - (__int128)sqrtD;
        if (num2 % 6 == 0 && num2 != num1)
            ++count;
    }

    return count;
}

/*
 * A214928  A209293 as table read layer by layer clockwise.
 * https://oeis.org/A214928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *  a(n) = (m1+m2-1)*(m1+m2-2)/2 + m1
 * where
 *  t  = floor(sqrt(n-1)) + 1
 *  i  = min(t, n - (t-1)^2)
 *  j  = min(t, t^2 - n + 1)
 *  m1 = floor((i+j)/2) + floor(i/2) * ((j%2==0) ? -1 : 1)
 *  m2 = ceil((i+j)/2) + floor(i/2) * ((j%2==1) ? -1 : 1)
 *
 * This implements the pairing‑function permutation of the natural
 * numbers described in OEIS A214928.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 0 ≤ n < 2^64 */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    /* Using a binary‑search algorithm based on bit shifts */
    idx_t bit = 1ULL << 62;           /* The second‑highest bit of a 64‑bit integer */
    idx_t result = 0;
    while (bit > n)
        bit >>= 2;                    /* Find the greatest power of four ≤ n */

    while (bit)
    {
        if (n >= result + bit)
        {
            n -= result + bit;
            result = (result >> 1) + bit;
        }
        else
            result >>= 1;
        bit >>= 2;
    }
    return result;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* the OEIS offset is 1; behaviour for 0 is undefined */
        return 0;

    /* t = floor(sqrt(n-1)) + 1 */
    idx_t t = floor_sqrt_u64(n - 1) + 1;

    /* (t-1)^2 and t^2 may overflow 64 bits, so use 128‑bit */
    __int128 t_minus = (__int128)t - 1;
    __int128 sq_t_minus = t_minus * t_minus;  /* (t-1)^2 */
    __int128 sq_t = (__int128)t * t;          /* t^2 */

    /* i = min(t, n - (t-1)^2) */
    idx_t rem_i = n - (idx_t)sq_t_minus;       /* 1 ≤ rem_i ≤ 2t-1 */
    idx_t i = rem_i < t ? rem_i : t;

    /* j = min(t, t^2 - n + 1) */
    idx_t rem_j = (idx_t)sq_t - n + 1;
    idx_t j = rem_j < t ? rem_j : t;

    /* Helper values */
    __int128 ii = i;
    __int128 jj = j;
    __int128 floor_half_sum = (ii + jj) / 2;          /* floor((i+j)/2) */
    __int128 ceil_half_sum  = (ii + jj + 1) / 2;      /* ceil((i+j)/2)  */
    __int128 floor_half_i   = ii / 2;                 /* floor(i/2) */

    /* Signs based on parity of j */
    int sign1 = (j % 2 == 0) ? -1 : 1;                 /* -(1) if j even */
    int sign2 = (j % 2 == 1) ? -1 : 1;                 /* -(1) if j odd */

    /* m1 and m2 */
    __int128 m1 = floor_half_sum + floor_half_i * sign1;
    __int128 m2 = ceil_half_sum  + floor_half_i * sign2;

    /* Final formula */
    __int128 sum_m = m1 + m2;
    __int128 res = (sum_m - 1) * (sum_m - 2) / 2 + m1;

    return (seq_t)res;
}

/*
 * A104572  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product A*B of the infinite lower triangular matrices A = [1;
 *          3, 1; 5, 3, 1; 7, 5, 3, 1; ...] and B=[1; 2,1; 1,2,1; 2,1,2,1;
 *          ...].
 * https://oeis.org/A104572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where T(i,j) = (i-j+1)*(3i-3j+2)/2 and (i,j) are
 *            such that n = (i-1)i/2 + j, 1 <= j <= i
 * Range:     u8 n <= 91, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of an unsigned __int128, floor result */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 res  = 0;
    unsigned __int128 bit = ((unsigned __int128)1) << 126;   /* 2^(2*63) */
    /* bring bit down to the greatest power of four <= x */
    while (bit > x) bit >>= 2;
    while (bit)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safe guard, offset is 1 */

    /* Find r = floor((sqrt(8*n+1)-1)/2) */
    unsigned __int128 t   = (unsigned __int128)8 * (unsigned __int128)n + 1;
    unsigned __int128 root = isqrt128(t);
    idx_t r = (idx_t)((root - 1) >> 1);    /* divide by 2 */

    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t i = (n == tri_r) ? r : r + 1;     /* row index */

    unsigned __int128 tri_prev = (unsigned __int128)(i - 1) * i / 2;
    idx_t j = (idx_t)(n - tri_prev);        /* column index */

    /* Compute T(i,j) = ((i-j+1)*(3*i-3*j+2))/2 */
    idx_t val1 = i - j + 1;                 /* i-j+1 */
    idx_t val2 = 3 * i - 3 * j + 2;         /* 3i-3j+2 */

    seq_t result = ((seq_t)val1 * (seq_t)val2) / 2;
    return result;
}

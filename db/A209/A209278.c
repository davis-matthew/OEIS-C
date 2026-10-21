/*
 * A209278  Second inverse function (numbers of rows) for pairing function
 *          A185180.
 * https://oeis.org/A209278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((t+3)/2) + floor(i/2)*(-1)^(i+t) where t =
 *            floor((sqrt(8*n-7)-1)/2), i = n - t*(t+1)/2
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit values: floor(sqrt(x)) */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)-1;       /* maximum 64‑bit value */
    idx_t ans  = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Guard against n==0 although offset is 1 */
    if (n == 0) return 0;

    /* Compute integer values t and i according to the formula */
    unsigned __int128 m = (unsigned __int128)n * 8 - 7;   /* 8*n-7  */
    idx_t      sqrt_m  = isqrt_u128(m);
    idx_t      t       = (sqrt_m - 1) >> 1;              /* floor((sqrt-1)/2) */
    unsigned __int128 tri_t   = (unsigned __int128)t * (t + 1) >> 1;  /* t*(t+1)/2 */
    idx_t      i        = (idx_t)(n - tri_t);                       /* i = n - tri_t */

    /* Parts of the final expression */
    seq_t part1 = ((seq_t)t + 3) / 2;          /* floor((t+3)/2) */
    seq_t part2 = ((seq_t)(i >> 1));          /* floor(i/2) */

    /* Parity determines the sign of part2 */
    if ((i + t) & 1) {
        return part1 - part2;                 /* odd parity → subtract */
    } else {
        return part1 + part2;                 /* even parity → add */
    }
}

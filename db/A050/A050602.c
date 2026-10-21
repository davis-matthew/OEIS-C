/*
 * A050602  Square array A(x,y), read by antidiagonals, where A(x,y) = 0 if (x
 *          AND y) = 0, otherwise A(x,y) = 1+A(x XOR y, 2*(x AND y)).
 * https://oeis.org/A050602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 8454, u16 n <= 8454, u32 n <= 8454, u64 n <= 8454,
 *            u128 n <= 8454, big any n
 * Helpers:   A_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A_calc.h"
/*   
 * Sequence A050602 – longest carry propagation length in binary addition.
 * a(n) is obtained by reading the symmetric square array A(x,y) by
 * antidiagonals:
 *    A(0,0), A(0,1), A(1,0), A(0,2), A(1,1), A(2,0), …
 * 
 * Recurrence (taken from the OEIS definition):
 *   A(x,y) = 0                     if (x & y) == 0
 *   A(x,y) = 1 + A(x ^ y, 2 * (x & y))   otherwise
 *
 * The function a(idx_t n) below implements this by:
 *   1. converting the linear index n to the pair (x,y) on the
 *      appropriate antidiagonal;
 *   2. iteratively evaluating the recurrence to obtain the value
 *      which is the carry‑propagation length.
 */

/* Integer square root for unsigned __int128 (returns floor(sqrt(x))) */
static __int128 isqrt128(__int128 x)
{
    __int128 r = 0;
    __int128 bit = (__int128)1 << 126;          /* highest power of 4 ≤ x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Find antidiagonal sum s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2   */
    __int128 eight_n_plus_one = (__int128)8 * n + 1;
    uint64_t sqrt_val = (uint64_t)isqrt128(eight_n_plus_one); /* floor(sqrt) */
    uint64_t s = (sqrt_val - 1) / 2u;

    /* 2. Compute k = n - s*(s+1)/2  (position within diagonal) */
    __int128 prev = (__int128)s * (s + 1) / 2;
    uint64_t k = (uint64_t)(n - (idx_t)prev);

    /* 3. Map to coordinates (x,y) : x = k, y = s - k                    */
    idx_t x = k;
    idx_t y = s - k;

    /* 4. Evaluate the recurrence */
    return A_calc(x, y);
}

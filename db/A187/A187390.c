/*
 * A187390  a(n) = floor(s*n), where s = 1 + sqrt(7) - sqrt(6); complement of
 *          A187389.
 * https://oeis.org/A187390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + sqrt(7) - sqrt(6)) * n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128: returns floor(sqrt(x)) */
static unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        /* avoid overflow in mid*mid by division */
        if (mid != 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* defined only for n >= 1, but guard to avoid UB */
        return (seq_t)0;

    unsigned __int128 N = n;
    /* A = 7 * N^2, B = 6 * N^2 */
    unsigned __int128 A = (unsigned __int128)7 * N * N;
    unsigned __int128 B = (unsigned __int128)6 * N * N;

    unsigned __int128 a_floor = isqrt_u128(A);
    unsigned __int128 b_floor = isqrt_u128(B);
    unsigned __int128 p = a_floor - b_floor;          /* floor difference */

    unsigned __int128 left = A - B - p * p;            /* A-B-p^2, always >=0 for n>=1 */
    unsigned __int128 right = (unsigned __int128)4 * p * p * B;  /* 4 * p^2 * B, >=0 */

    /* Decide whether floor(D) is p-1 or p, avoiding overflow:
       condition: left^2 < right   ⇔   left < right / left (if left>0)   */
    int subtract_one = 0;
    if (left == 0) {
        subtract_one = 0;                     /* left^2 = 0, right >=0, so false */
    } else if (left < right / left) {
        subtract_one = 1;                     /* left^2 < right → floor(D)=p-1 */
    }

    unsigned __int128 u = subtract_one ? (p - 1) : p;   /* floor(n*sqrt7 - n*sqrt6) */
    unsigned __int128 result = N + u;                  /* final a(n) */

    return (seq_t)result;
}

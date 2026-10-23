/*
 * A131227  2*A051340 - A128174.
 * https://oeis.org/A131227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A131227 – entry a(n) in the lower‑triangular array
 *  defined by:
 *      •  Row r (r ≥ 0) has r+1 elements.
 *      •  The last element of row r equals 2·r+1.
 *      •  The preceding elements alternate 1,2,1,2,…,
 *          starting with 1 when r is even and with 2 when r is odd.
 *
 *  In row–major order, a(0)=1; a(1)=2; a(2)=3; a(3)=1; a(4)=2; a(5)=5; …  */
/*  This function is O(log n) in the worst case due to the integer sqrt
 *  computation, and uses only integer arithmetic.  */

#include <stdint.h>

static inline unsigned __int128 triangular(idx_t k)
{
    /* returns k*(k+1)/2 as 128‑bit integer to avoid overflow            */
    return (unsigned __int128)k * (k + 1) / 2;
}

static inline idx_t isqrt_u128(unsigned __int128 x)
{
    /* integer square root of a 128‑bit value using binary search.
     * The result fits in 64 bits because x ≤ 2^66 for our use case.      */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 33;  /* 2^33 > sqrt(2^66)                         */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* 1. find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
     *    Using the quadratic formula r = floor((sqrt(8n+1)-1)/2).          */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;   /* 8*n+1  */
    idx_t sqrt_val = isqrt_u128(val);
    idx_t r = (sqrt_val - 1) >> 1;                         /* (sqrt-1)/2 */

    /* 2. find column c (0‑based) within row r */
    unsigned __int128 tr = triangular(r);
    idx_t c = n - (idx_t)tr;

    /* 3. compute value according to the pattern described above */
    if (c == r) {
        /* last element of the row */
        return (seq_t)(2 * r + 1);
    } else {
        /* alternating 1 / 2, starting with 1 if r even, 2 if r odd     */
        if (((r ^ c) & 1ULL) == 0)
            return (seq_t)1;
        else
            return (seq_t)2;
    }
}

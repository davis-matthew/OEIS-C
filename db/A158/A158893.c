/*
 * A158893  Triangle read by rows: T(n,1)=7n-6; T(n,m)= 1+n-m, 1<m<=n.
 * https://oeis.org/A158893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,1)=7n-6; T(n,m)=1+n-m for 1<m<=n. a(n) is the n‑th element
 *            read row by row from that triangle
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) for 0 <= x <= 2^67.  The result fits into 64 bits. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 36;          /* 2^36 > sqrt(2^67) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th integer. */
    unsigned __int128 disc = (unsigned __int128)8 * n + 1;
    idx_t      sqrtDisc = isqrt_u128(disc);
    idx_t      r        = (sqrtDisc - 1) >> 1;           /* floor root */

    /* Correct to the smallest r with r*(r+1)/2 >= n. */
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* Position within the row (1‑based). */
    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2); /* elements before this row */
    idx_t pos  = n - prev;                                 /* 1 ≤ pos ≤ r */

    if (pos == 1)
        return (seq_t)((seq_t)7 * (seq_t)r - 6);           /* first element */
    else
        return (seq_t)(1 + (seq_t)r - (seq_t)pos);        /* other elements */
}

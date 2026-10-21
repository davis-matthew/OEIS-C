/*
 * A067589  Numbers k such that A067588(k) is an odd number.
 * https://oeis.org/A067589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(3*k-1)/2 where k is defined by * m = (n-1)/4 * r =
 *            (n-1)%4 * if r==0 : k = 4*m+1 * if r==1 : k = 4*m+2 * if r==2 :
 *            k = -(4*m+2) * if r==3 : k = -(4*m+3) * * This k sequence
 *            generates all odd generalized pentagonal numbers * sorted in
 *            increasing order
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1, so n-1 is safe */
    idx_t idx = n - 1;
    idx_t m   = idx / 4;
    idx_t r   = idx % 4;
    int64_t k;

    if (r == 0)          k = (int64_t)(4 * m + 1);
    else if (r == 1)     k = (int64_t)(4 * m + 2);
    else if (r == 2)     k = -(int64_t)(4 * m + 2);
    else /* r == 3 */    k = -(int64_t)(4 * m + 3);

    /* compute k*(3*k-1)/2 using signed 128‑bit to avoid overflow */
    __int128 numer = (__int128)k * (((__int128)3 * k) - 1);
    __int128 result128 = numer / 2;
    return (seq_t)result128;
}

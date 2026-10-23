/*
 * A166282  Matrix inverse of Sierpinski's triangle (A047999, Pascal's
 *          triangle mod 2).
 * https://oeis.org/A166282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 0 if (k & ~m) != 0; otherwise a(n) =
 *            (-1)^(popcnt(m)-popcnt(k)), where m = ⌊(√(8n+1)-1)/2⌋ and k = n
 *            - m*(m+1)/2
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the linearization of the inverse of Sierpinski's triangle
   (the Möbius matrix for the subset lattice). */

#include <stdint.h>
#include <stdbool.h>

/* integer square root (floor) for a 64‑bit unsigned value */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if (((unsigned __int128)mid * mid) <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* parity (popcount mod 2) of an unsigned integer */
static int parity(idx_t x)
{
    int p = 0;
    while (x)
    {
        p ^= 1;
        x &= x - 1;   /* drop lowest set bit */
    }
    return p;
}

seq_t a(idx_t n)
{
    /* compute row index m */
    idx_t u = 1 + (n << 3);          /* 1 + 8*n  */
    idx_t s = isqrt_u64(u);
    idx_t m = (s - 1) >> 1;          /* floor((s-1)/2) */

    /* adjust in case of off–by–one from the integer sqrt */
    while ( ((unsigned __int128)m * (m + 1)) / 2 > n )
        --m;
    while ( ((unsigned __int128)(m + 1) * (m + 2)) / 2 <= n )
        ++m;

    /* column index k */
    idx_t start = ((unsigned __int128)m * (m + 1)) / 2;
    idx_t k = n - start;

    /* if k is not a subset of m, entry is 0 */
    if ((k & ~m) != 0)
        return (seq_t)0;

    /* compute sign based on parity difference */
    int pm = parity(m);
    int pk = parity(k);
    return (seq_t)((pm ^ pk) ? -1 : 1);
}

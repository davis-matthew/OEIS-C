/*
 * A121489  Diagonal of array A121490.
 * https://oeis.org/A121489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/2)+1)^2 - n^3
 * Range:     u8 n <= 28, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest positive integer a such that n^3 + a is a square. */

#include <stdint.h>   /* for explicit integer types, optional */

/* Integer square root for idx_t values (floor of sqrt(x)). */
static idx_t int_sqrt(idx_t x)
{
    idx_t lo   = 0;
    idx_t hi   = x;
    idx_t res  = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == x)      return mid;
        if (sq < x)       { res = mid; lo = mid + 1; }
        else              hi = mid - 1;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    idx_t n_l   = n;
    idx_t n3    = n_l * n_l * n_l;       /* n^3, fits in idx_t for n up to ~10^6 */
    idx_t t     = int_sqrt(n3);          /* floor(sqrt(n^3)) */
    idx_t t1    = t + 1;                 /* floor(n^(3/2)+1) */
    idx_t diff  = t1 * t1 - n3;          /* (t+1)^2 - n^3 */

    return (seq_t)diff;
}

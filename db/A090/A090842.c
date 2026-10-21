/*
 * A090842  Square array of numbers read by antidiagonals where T(n,k) =
 *          ((k+3)*(k+2)^n-2)/(k+1).
 * https://oeis.org/A090842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090842: a(n) is the antidiagonal reading of the array
   T(n,k) = ((k+3)*(k+2)^n - 2)/(k+1),  with n,k >= 0. */
#include <stdint.h>
#include <math.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Map index n to the array cell (row = exp, col = k) by
       antidiagonal enumeration.  The diagonal number s satisfies
       s*(s+1)/2 <= n < (s+1)*(s+2)/2.  */
    idx_t s = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);

    /* Adjust for any rounding error caused by the floating point
       calculation of s.  (s is small, so 128‑bit arithmetic is safe.) */
    while (((unsigned __int128)(s + 1) * (s + 2)) / 2 <= n)
        s++;
    while (s > 0 && ((unsigned __int128)s * (s + 1)) / 2 > n)
        s--;

    idx_t before = ((unsigned __int128)s * (s + 1)) / 2;
    idx_t pos = n - before;              /* pos in [0..s] */
    idx_t exp = pos;                     /* row index n */
    idx_t k   = s - pos;                 /* column index k */

    seq_t base  = (seq_t)(k + 2);
    seq_t power = pow_seq_t(base, exp);

    seq_t numerator = ((seq_t)(k + 3)) * power - (seq_t)2;
    seq_t result    = numerator / (seq_t)(k + 1);
    return result;
}

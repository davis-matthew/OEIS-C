/*
 * A088828  Nonsquare positive odd numbers.
 * https://oeis.org/A088828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence:  A088828 – Nonsquare positive odd numbers
 * Formula:  a(n) = 2*n + s - ((s+1) mod 2)
 *          where s = round( sqrt(2*n-1) )
 *  (round means nearest integer, ties rounded up).
 *
 * The function below implements this formula using only integer
 * arithmetic, so it works for any integer type seq_t (8, 16, 32,
 * 64, 128 bits or arbitrary precision).
 */

#include <stdbool.h>

static seq_t _floor_sqrt(seq_t x)
{
    /* Integer square root: largest r with r*r <= x (x >= 0). */
    seq_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1, but guard anyway */
        return 0;

    seq_t N = (seq_t)n;          /* cast index to seq_t */
    seq_t k = (2 * N) - 1;       /* 2*n - 1, always odd and >0 */

    seq_t f = _floor_sqrt(k);    /* floor(sqrt(k)) */
    /* determine whether to round up or down */
    seq_t s = (k < f * f + f + 1) ? f : f + 1;   /* round(sqrt(k)) */

    seq_t res = (2 * N) + s - ((s + 1) & 1UL);
    return res;
}

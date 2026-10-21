/*
 * A075300  Array A read by antidiagonals upwards: A(n, k) = array
 *          A054582(n,k) - 1 = 2^n*(2*k+1) - 1 with n,k >= 0.
 * https://oeis.org/A075300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the array A(n,k) = 2^n*(2k+1)-1
 *            along antidiagonals upwards (rows run from 0)
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2.
       For small n (<1e4) a floating‑point approximation is safe. */
    idx_t s = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* Correct possible rounding errors */
    while (s * (s + 1) / 2 > n) --s;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;

    /* Position inside the diagonal */
    idx_t pos  = n - s * (s + 1) / 2;
    /* Row decreases from s to 0; column increases from 0 to s */
    idx_t row = s - pos;   /* n index */
    idx_t col = pos;       /* k index */

    /* Compute 2^row*(2*col+1)-1 using seq_t arithmetic */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < row; ++i)
        pow2 = pow2 * (seq_t)2;     /* safe: row <= 10000 for test limits */

    seq_t val = pow2 * ((seq_t)2 * (seq_t)col + (seq_t)1) - (seq_t)1;
    return val;
}

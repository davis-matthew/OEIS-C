/*
 * A068607  Triangle of T(n,k)=n*k*(n+k+1) with n>=k>=0.
 * https://oeis.org/A068607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r * c * (r + c + 1) where * r = floor((sqrt(8*n + 1) - 1)
 *            / 2) * c = n - r*(r + 1)/2 * the sequence enumerates the rows of
 *            the triangle * T(i,j) = i*j*(i + j + 1) in row-major order
 * Range:     u8 n <= 19, u16 n <= 559, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find r = floor((sqrt(8*n + 1) - 1) / 2) */
    long double x = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((x - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one due to floating point error. */
    /* Use 128‑bit arithmetic to avoid overflow when checking. */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Compute c = n - r*(r + 1)/2 */
    idx_t as = (unsigned __int128)r * (r + 1) / 2;
    idx_t c  = n - as;

    /* Calculate the triangle value in seq_t, using seq_t width for precision. */
    seq_t sr = (seq_t)r;
    seq_t sc = (seq_t)c;
    seq_t sum = sr + sc + 1;
    seq_t result = sr * sc;
    result *= sum;

    return result;
}

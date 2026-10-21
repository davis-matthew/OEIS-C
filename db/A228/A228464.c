/*
 * A228464  Number of arrays of maxima of three adjacent elements of some 0..n
 *          array of length 9.
 * https://oeis.org/A228464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(n+2)*(32*n^5 + 408*n^4 + 3808*n^3 + 7605*n^2 +
 *            5367*n + 1260)/2520
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of OEIS A228464 (offset 1). */
seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 1.  For safety, return 0 when n==0. */
    if (n == 0)
        return 0;

    /* Use 128‑bit arithmetic for the intermediate calculations.
       GCC and Clang provide the `unsigned __int128` type. */
    unsigned __int128 N = n;                     /* n as 128‑bit */
    unsigned __int128 poly = 32;                 /* start of the 5th‑degree polynomial */

    /* Horner's rule: poly = 32*n^5 + 408*n^4 + 3808*n^3 + 7605*n^2 + 5367*n + 1260 */
    poly = poly * N + 408;
    poly = poly * N + 3808;
    poly = poly * N + 7605;
    poly = poly * N + 5367;
    poly = poly * N + 1260;

    /* Numerator: (n+1)*(n+2)*poly */
    unsigned __int128 numerator = (N + 1) * (N + 2) * poly;

    /* Division by 2520 (2^3 * 3^2 * 5 * 7).  The division is exact. */
    numerator /= 2520;

    return (seq_t)numerator;
}

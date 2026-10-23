/*
 * A185874  Second accumulation array of A051340, by antidiagonals.
 * https://oeis.org/A185874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n,k) where T(n,k) = k*n*(n+1)*(2*n+3*k+1)/12 (n,k) are
 *            the indices of the n-th element when the lower‑right triangular
 *            array T(n,k) (n,k ≥ 1) is read along antidiagonals with sum s =
 *            n + k increasing, starting from s = 2
 * Range:     u8 n <= 8, u16 n <= 315, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* n is expected to be ≥ 1 */

    /* find minimal s such that s*(s-1)/2 ≥ n */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;           /* 8*n+1 fits 128‑bit */
    long double d = sqrtl((long double)val);
    idx_t s = (idx_t)((1 + d) / 2.0L);                           /* initial guess */

    while ((s - 1) * s / 2 < n) ++s;                             /* correction */

    /* offset within the current antidiagonal (1‑based) */
    idx_t prev = (s - 2) * (s - 1) / 2;                          /* sum up to s-1 */
    idx_t offset = n - prev;                                     /* 1 ≤ offset ≤ s-1 */

    idx_t nn = offset;          /* n-coordinate in T(n,k) */
    idx_t kk = s - offset;      /* k-coordinate in T(n,k) */

    seq_t N = (seq_t)nn;
    seq_t K = (seq_t)kk;

    seq_t result = K * N;
    result *= (N + 1);
    result *= (2 * N + 3 * K + 1);
    result /= 12;                /* division exact by construction */

    return result;
}

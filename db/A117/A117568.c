/*
 * A117568  Riordan array ((1-x^3)/(1+x^2),x).
 * https://oeis.org/A117568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the n-th element of the flattened Riordan triangle with
 *            g(x) = (1 - x^3)/(1 + x^2) and f(x) = x. The triangle entry
 *            T(n,k) = c_{n-k} where the coefficient sequence c_m of g(x) is:
 *            c0 = 1, c1 = 0, c_m = (-1)^(floor(m/2)) for m ≥ 2 (*pattern:
 *            1,0,-1,-1,1,1,-1,-1,...*). The sequence a(n) lis...
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r and the offset t = r(r+1)/2 such that
       t ≤ n < t + (r + 1).  Use only integer arithmetic. */
    idx_t r = 0;
    idx_t t = 0;                   /* t = current triangular number T(r) */
    while (t + r + 1 <= n) {       /* t + (r+1) = T(r+1) */
        t += r + 1;
        ++r;
    }
    idx_t k = n - t;                /* position within the row */
    idx_t m = r - k;                /* index in the coefficient sequence */

    if (m == 0) return (seq_t)1;    /* c0 */
    if (m == 1) return (seq_t)0;    /* c1 */

    /* Pattern for m ≥ 2:  [-1, -1, 1, 1] repeating. */
    static const int pattern[4] = { -1, -1, 1, 1 };
    return (seq_t)pattern[(m - 2) % 4];
}

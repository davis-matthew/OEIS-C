/*
 * A156241  Triangle read by rows, T(n,k). If k divides n then
 *          A008683(n)*A008683(k).
 * https://oeis.org/A156241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Triangle read by rows, T(n,k) = μ(n)*μ(k) if k|n else 0,
   with diagonal entries set to 1.
   a(n) returns the n‑th entry in this triangular enumeration.
*/

#include <stdint.h>

static seq_t mobius(idx_t n)
{
    if (n == 1) return 1;
    int factors = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            if (cnt > 1) return 0;           /* square factor */
            ++factors;
        }
    }
    if (m > 1) ++factors;                     /* remaining prime factor */
    return (factors & 1) ? -1 : 1;           /* (-1)^factors */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; behaviour below not used */

    /* find the row r: smallest r with r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    idx_t prev = r * (r - 1) / 2;             /* total terms before row r */
    idx_t k = n - prev;                       /* column index in the row */

    if (k == r)                              /* diagonal entry is 1 by definition */
        return 1;

    if (r % k != 0)                          /* k does not divide r */
        return 0;

    seq_t mu_r = mobius(r);
    seq_t mu_k = mobius(k);
    return mu_r * mu_k;
}

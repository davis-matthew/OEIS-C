/*
 * A145598  Triangular array of generalized Narayana numbers: T(n, k) =
 *          4*binomial(n+1, k+3)*binomial(n+1, k-1)/(n+1).
 * https://oeis.org/A145598
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k satisfy n-3 = (r-3)(r-2)/2 + (k-1)
 *            (with r >= 3, 1 <= k <= r-2) and T(r,k) = 4 * binomial(r+1, k+3)
 *            * binomial(r+1, k-1) / (r+1)
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;           /* undefined for n < 3 */

    idx_t idx = n - 3;              /* zero–based linear index */
    /* Find greatest s such that s*(s+1)/2 <= idx  (triangular numbers) */
    idx_t low = 0, high = idx + 1, s = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = (mid * (mid + 1)) >> 1;
        if (tri <= idx) {
            s = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    idx_t r = s + 3;                       /* row index */
    idx_t offset = (s * (s + 1)) >> 1;     /* entries before row r */
    idx_t k = idx - offset + 1;            /* column index (1‑based) */

    /* Compute T(r,k) = 4*C(r+1,k+3)*C(r+1,k-1)/(r+1) */
    seq_t a1 = binomial(r + 1, k - 1);
    seq_t a2 = binomial(r + 1, k + 3);
    seq_t den = r + 1;

    /* Reduce fraction before multiplication to avoid overflow */
    seq_t g = gcd(a1, den);
    a1 /= g;
    den /= g;

    g = gcd(a2, den);
    a2 /= g;
    den /= g;   /* den now divides 1 in the exact division case */

    seq_t res = a1 * a2;
    res *= 4;      /* multiply by 4 */

    if (den != 1) res /= den;   /* safety, den should be 1 */
    return res;
}

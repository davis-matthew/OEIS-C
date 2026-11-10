/*
 * A050175  T(n,k)=S(n,k,k-3), 2<=k<=n-3, n >= 5, array S as in A050157.
 * https://oeis.org/A050175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = C(n,k) - C(n,2) for 3<=k<=n-3; T(n,2) = C(n-1,2) -
 *            (n-1) (paths from (0,0) to (n-k,k) kept under the line,
 *            reflection principle); a(m) lists the triangle by rows n>=5,
 *            k=2..n-3
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-07)
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/isqrt.h"

seq_t a(idx_t m)
{
    idx_t j = (isqrt(8 * m + 1) - 1) / 2;
    while (j * (j + 1) / 2 > m) j--;
    while ((j + 1) * (j + 2) / 2 <= m) j++;
    idx_t c = m - j * (j + 1) / 2;
    idx_t n = j + 5;
    idx_t k = c + 2;
    if (k == 2) {
        return binomial(n - 1, 2) - binomial(n - 1, 1);
    }
    return binomial(n, k) - binomial(n, 2);
}

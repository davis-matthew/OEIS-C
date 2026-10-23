/*
 * A156603  Square array T(n, k) = Product_{j=1..n} p(j, k+1), p(n, x) =
 *          Sum_{j=0..n} (-1)^j*A053122(n, j)*x^j, and T(n, 0) = n!, read by
 *          antidiagonals.
 * https://oeis.org/A156603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is read by antidiagonals of the square array T. T(n,0)=n!
 *            for n>=0. For k>=1, T(n,k)=∏_{j=0}^{n-1} (-1)^j U(j,(k-1)/2),
 *            where U(j,x) is the Chebyshev polynomial of the second kind
 *            defined by U_0=1, U_1=2x, U_{j+1}=2x U_j - U_{j-1}
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 83, i64 n <= 112,
 *            i128 n <= 181, big any n
 * Helpers:   _floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary helper: integer square root of an unsigned 64‑bit number */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"      /* to satisfy compiler - any utility included is allowed */

#if __has_include("../utils/_floor_isqrt_u64.h")
#include "../utils/_floor_isqrt_u64.h"
#else
/* Fallback implementation (should not be needed on the target build). */
static inline uint64_t _floor_isqrt_u64(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x + 1) >> 1;
    while (y < x) { x = y; y = (x + n / x) >> 1; }
    return x;
}
#endif

static seq_t factorial(idx_t m)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= m; ++i)
        f *= (seq_t)i;
    return f;
}

seq_t a(idx_t n)
{
    /* Determine the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = (_floor_isqrt_u64(8ULL * n + 1ULL) - 1ULL) / 2ULL;
    idx_t start = d * (d + 1) / 2;
    idx_t offset = n - start;           /* row index (m) */
    idx_t m = offset;
    idx_t k = d - offset;              /* column index */

    /* Handle special case k==0 */
    if (k == 0)
        return factorial(m);

    /* Compute product over j = 0 .. m-1 of (-1)^j * U(j,(k-1)/2) */
    seq_t prod = 1;
    seq_t u_prev = 1;                   /* U_0 = 1 */
    seq_t u_curr = (seq_t)(k - 1);      /* U_1 = k-1 = 2x */

    if (m >= 1)                      /* j = 0, sign = +1 */
        prod *= u_prev;

    if (m >= 2)                      /* j = 1, sign = -1 */
        prod *= -u_curr;

    for (idx_t j = 2; j < m; ++j)
    {
        seq_t u_next = (seq_t)(k - 1) * u_curr - u_prev;
        if ((j & 1) == 0)            /* even j: sign = +1 */
            prod *= u_next;
        else                        /* odd j: sign = -1 */
            prod *= -u_next;
        u_prev = u_curr;
        u_curr = u_next;
    }

    return prod;
}

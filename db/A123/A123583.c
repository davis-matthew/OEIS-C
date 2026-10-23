/*
 * A123583  Triangle read by rows: T(n, k) is the coefficient of x^k in the
 *          polynomial 1 - T_n(x)^2, where T_n(x) is the n-th Chebyshev
 *          polynomial of the first kind.
 * https://oeis.org/A123583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(n) = ((k==0 ? 1 : 0) - coeff_T(2r,k)) / 2 where (r,k) are the
 *            row and column indices in the triangle read by rows, and
 *            coeff_T(m,k) is the coefficient of x^k in the m‑th Chebyshev
 *            polynomial of the first kind, with m even. The Chebyshev
 *            coefficients are obtained by the closed‑form expression fr...
 * Range:     i8 n <= 19, i16 n <= 58, i32 n <= 213, i64 n <= 762,
 *            i128 n <= 2499, big any n
 * Helpers:   _floor_isqrt_u64, binom_n_k, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/binom_n_k.h"
#include "../utils/pow2.h"
#include "../utils/_floor_isqrt_u64.h"

static seq_t coeff_T_even(idx_t m, idx_t k)
{
    /* m is even, 0 <= k <= m, and m,k are of type idx_t. */
    if (((m - k) & 1U) != 0U)
        return (seq_t)0;          /* odd exponents are absent */

    idx_t j = (m - k) >> 1U;      /* j = (m - k)/2 */

    /* binom(m-j, j) */
    seq_t bin = binom_n_k(m - j, j);

    /* c = bin * m / (m - j) */
    seq_t c = bin * (seq_t)m;
    c = c / (seq_t)(m - j);

    /* Multiply by 2^(m - 2j - 1) unless m == 2j (constant term) */
    if (m == 2U * j)                /* exponent would be –1 */
        c = c / (seq_t)2U;          /* divide by 2 exactly */
    else {
        idx_t exp = m - 2U * j - 1U;
        c = c * pow2(exp);
    }

    /* Apply sign: (–1)^j */
    if (j & 1U)
        c = -c;

    return c;
}

seq_t a(idx_t n)
{
    /* For n == 0 the triangle row 0, col 0 is 0. */
    if (n == 0)
        return (seq_t)0;

    /* Determine row r and column k in the concatenated triangle. */
    idx_t r = _floor_isqrt_u64(n);        /* floor(sqrt(n)) */
    idx_t k = n - r * r;                  /* offset within row r */

    /* Compute coefficient of x^k in T_{2r}(x). */
    idx_t m = r << 1U;                    /* m = 2r (even) */
    seq_t coeff_T = coeff_T_even(m, k);

    /* Coefficient of x^k in (1 – T_{2r})/2  */
    seq_t delta = (k == 0U) ? (seq_t)1 : (seq_t)0;
    return (delta - coeff_T) / (seq_t)2U;
}

/*
 * A015981  Inverse of 1972nd cyclotomic polynomial.
 * https://oeis.org/A015981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of 1 /
 *            φ₁₉₇₂(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The cyclotomic polynomial φ₁₉₇₂(x) factors into (xᵈ – 1)ᵏ with exponents ±1 */
/* for d = 2, 4, 34, 58, 68, 116, 986, 1972. The algorithm multiplies the
 * generating functions (1 – xᵈ) or 1 / (1 – xᵈ) accordingly. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate coefficient array up to index n */
    /* cast malloc result for C++ arbitrary‑precision seq_t,            */
    /*   while remaining compliant with C11 and C++17.                  */
    seq_t *buf = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!buf) return (seq_t)0;             /* handle allocation failure */

    /* initialise with 1 at x⁰, all other coefficients 0 */
    for (size_t i = 0; i <= (size_t)n; ++i) buf[i] = (seq_t)0;
    buf[0] = (seq_t)1;

    /* divisors of 1972 with non‑zero exponents (±1) */
    static const uint16_t divisors[] =
        {2, 4, 34, 58, 68, 116, 986, 1972};
    static const int exps[] =
        { 1, -1, -1, -1,  1,  1,  1, -1};
    const size_t K = sizeof(divisors) / sizeof(divisors[0]);

    /* cast index n to signed int for loop counters */
    int N = (int)n;

    for (size_t idx = 0; idx < K; ++idx) {
        int d = (int)divisors[idx];
        int e = exps[idx];

        if (e == 1) {               /* multiply by (1 – x^d) */
            for (int k = N; k >= d; --k)
                buf[k] -= buf[k - d];
        } else {                    /* multiply by 1 / (1 – x^d) = 1 + x^d + x^{2d} + … */
            for (int k = 0; k <= N; ++k)
                if (k >= d) buf[k] += buf[k - d];
        }
    }

    seq_t res = buf[N];
    free(buf);
    return res;
}

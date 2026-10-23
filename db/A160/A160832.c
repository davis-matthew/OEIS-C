/*
 * A160832  Expansion of eta(q)*eta(q^2)*eta(q^4), where eta(q) =
 *          Product((1-q^m), m=1..oo).
 * https://oeis.org/A160832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of qⁿ in η(q)·η(q²)·η(q⁴), where η(q) =
 *            ∏_{m≥1}(1‑q^m). The generating – function equals
 *            ∏_{m≥1}(1‑q^m)(1‑q^{2m})(1‑q^{4m})
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code builds the coefficients by convolving the current
    polynomial with the kernel 1‑q^k for k = m, 2m, and 4m
    for each m = 1…n.  It works for any build where seq_t
    supports the usual + – * / operators.  It uses malloc/free
    which are compatible with C11 and C++17 (seq_t is assumed
    to be trivially destructible). */

#include <stdlib.h>

/* helper: convert fixed index to array size */
#if defined(_MSC_VER)
  #define ARRAY_SIZE(n) ((size_t)(n)+1)
#else
  #define ARRAY_SIZE(n) ((size_t)(n)+1)
#endif

seq_t a(idx_t n)
{
    /* Allocate and zero the coefficient array */
    seq_t *c = (seq_t *)calloc(ARRAY_SIZE(n), sizeof(seq_t));
    if (!c) return 0;          /* allocation failure – return 0 */
    c[0] = 1;                   /* coefficient of q⁰ */

    /* For each m from 1 to n, apply the three factors
       (1‑q^m), (1‑q^{2m}) and (1‑q^{4m}).  */
    for (idx_t m = 1; m <= n; ++m)
    {
        /* factor (1‑q^m) */
        for (idx_t i = n; i >= m; --i)
            c[i] -= c[i - m];

        /* factor (1‑q^{2m}) – only if the shift fits */
        if (m <= n / 2) {
            idx_t shift = 2 * m;
            for (idx_t i = n; i >= shift; --i)
                c[i] -= c[i - shift];
        }

        /* factor (1‑q^{4m}) – only if the shift fits */
        if (m <= n / 4) {
            idx_t shift = 4 * m;
            for (idx_t i = n; i >= shift; --i)
                c[i] -= c[i - shift];
        }
    }

    seq_t result = c[n];
    free(c);
    return result;
}

/*
 * A153038  Denominators of the fixed point a=(a_1,a_2,...) of the
 *          transformation x'= fix(x) where fix(x)_n = Sum_{d|n} d x_d, i.e.,
 *          fix(a)=a.
 * https://oeis.org/A153038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, eigen, frac, mult, sign
 * Formula:   a(n) = ∏_{p^r‖n} ∏_{j=1..r} (1 - p^j)
 * Range:     i8 n <= 15, i16 n <= 63, i32 n <= 255, i64 n <= 2047,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined by the build system */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;                  /* a(1) = 1 */

    uint64_t m = (uint64_t)n;       /* work on an unsigned 64‑bit copy */
    seq_t result = 1;

    /* trial division – sufficient for n up to about 10 000 */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            seq_t pow = (seq_t)p;   /* current power p^j (initially j=1) */
            for (int j = 1; j <= exp; ++j) {
                result *= ((seq_t)1 - pow); /* multiply by (1 - p^j) */
                pow *= p;                  /* advance to p^(j+1) */
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t pow = (seq_t)m;
        result *= ((seq_t)1 - pow);     /* (1 - p) for the last prime */
    }

    return result;
}

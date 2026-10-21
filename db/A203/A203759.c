/*
 * A203759  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (1,1,2,2,...,[n/2]).
 * https://oeis.org/A203759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1 <= i < j <= n} (x_i^2 + x_i*x_j + x_j^2) with
 *            x_k = (k+1)/2. See A203759 definition
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;

    idx_t half = n / 2;                /* floor(n/2) */
    /* self-pairs: each value appears twice (except maybe last). */
    for (idx_t u = 1; u <= half; ++u) {
        seq_t uu = (seq_t)u * (seq_t)u;
        result *= 3 * uu;              /* f(u,u) = 3u^2 */
    }

    /* distinct pairs among the first half, exponent 4. */
    for (idx_t u = 1; u <= half; ++u) {
        seq_t uu = (seq_t)u * (seq_t)u;
        for (idx_t v = u + 1; v <= half; ++v) {
            seq_t vv = (seq_t)v * (seq_t)v;
            seq_t uv = (seq_t)u * (seq_t)v;
            seq_t f = uu + uv + vv;    /* u^2 + uv + v^2 */

            /* f^4 = (f^2)^2 */
            seq_t f2 = f * f;
            seq_t f4 = f2 * f2;
            result *= f4;
        }
    }

    /* if n is odd, handle the last distinct value only once. */
    if (n % 2 == 1) {
        idx_t M = half + 1;            /* last value */
        seq_t MM = (seq_t)M * (seq_t)M;
        for (idx_t u = 1; u <= half; ++u) {
            seq_t uu = (seq_t)u * (seq_t)u;
            seq_t uv = (seq_t)u * (seq_t)M;
            seq_t f = uu + uv + MM;    /* u^2 + u*M + M^2 */

            seq_t f2 = f * f;          /* exponent 2 for cross pairs */
            result *= f2;
        }
    }

    return result;
}

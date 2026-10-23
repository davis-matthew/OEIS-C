/*
 * A159421  Number of n X n arrays of squares of integers summing to 28.
 * https://oeis.org/A159421
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^28 in (1 + t + t^4 + t^9 + t^16 +
 *            t^25)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two polynomials truncated to degree 28 */
static void poly_mul(const seq_t *a, const seq_t *b, seq_t *out) {
    seq_t res[29];
    for (int i = 0; i <= 28; ++i) res[i] = 0;
    for (int i = 0; i <= 28; ++i) {
        if (a[i] == 0) continue;
        for (int j = 0; j <= 28 - i; ++j) {
            if (b[j] == 0) continue;
            res[i + j] += a[i] * b[j];
        }
    }
    for (int i = 0; i <= 28; ++i) out[i] = res[i];
}

seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* offset is 2 */
    idx_t m = (idx_t)n * n;               /* exponent: n^2 */

    seq_t base[29] = {0};
    base[0] = 1;  /* t^0 */
    base[1] = 1;  /* t^1 */
    base[4] = 1;  /* t^4 */
    base[9] = 1;  /* t^9 */
    base[16] = 1; /* t^16 */
    base[25] = 1; /* t^25 */

    seq_t result[29] = {0};
    result[0] = 1;                     /* polynomial 1 */

    seq_t tmp[29];

    while (m > 0) {
        if (m & 1) {
            poly_mul(result, base, tmp);
            for (int i = 0; i <= 28; ++i) result[i] = tmp[i];
        }
        poly_mul(base, base, tmp);
        for (int i = 0; i <= 28; ++i) base[i] = tmp[i];
        m >>= 1;
    }
    return result[28];
}

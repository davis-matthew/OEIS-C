/*
 * A159427  Number of n X n arrays of squares of integers summing to 31.
 * https://oeis.org/A159427
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^31 in (1+x+x⁴+x⁹+x¹⁶+x²⁵)^(n²)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void poly_mul(const seq_t *a, const seq_t *b, seq_t *res) {
    size_t i, j;
    for (i = 0; i < 32; ++i) res[i] = 0;
    for (i = 0; i < 32; ++i) {
        if (!a[i]) continue;
        for (j = 0; j < 32 - i; ++j) {        /* i + j <= 31 */
            if (b[j]) res[i + j] += a[i] * b[j];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    idx_t exp = n * n;          /* exponent n² */
    seq_t res[32]  = {0};
    seq_t base[32] = {0};
    seq_t tmp[32];

    res[0] = 1;                          /* (1)^exp = 1 */

    base[0] = 1;  base[1] = 1;  base[4] = 1;
    base[9] = 1;  base[16] = 1; base[25] = 1;   /* polynomial P(x) */

    while (exp > 0) {
        if (exp & 1) {
            poly_mul(res, base, tmp);
            for (size_t i = 0; i < 32; ++i) res[i] = tmp[i];
        }

        poly_mul(base, base, tmp);
        for (size_t i = 0; i < 32; ++i) base[i] = tmp[i];

        exp >>= 1;
    }

    return res[31];
}

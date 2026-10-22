/*
 * A000501  a(n) = floor(cosh(n)).
 * https://oeis.org/A000501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(cosh(n)) = floor(Sum_{j>=0} n^(2j)/(2j)!),
 *            evaluated exactly in integer fixed point (Horner form,
 *            truncating) with enough guard bits
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   a501_div, a501_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a501_div.h"
#include "../utils/a501_mul.h"

seq_t a(idx_t n)
{
    uint64_t nb = (uint64_t)n * 1443 / 1000;       /* ~ log2(e^n) */
    size_t fw = (size_t)((nb + 70) / 32 + 1);       /* fractional limbs */
    size_t iw = (size_t)((nb + 2) / 32 + 2);        /* integer limbs */
    size_t L = fw + iw;
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t J = (3 * (uint64_t)n + 200) / 2 + 1;
    uint64_t j;
    seq_t r = 0;
    size_t i;
    int b;

    x[fw] = 1; /* value 1 */
    for (j = J; j >= 1; j--) {
        uint64_t d1 = 2 * j - 1, d2 = 2 * j;
        if (n < 65536) {
            a501_mul(x, L, (uint64_t)n * n);
        } else {
            a501_mul(x, L, (uint64_t)n);
            a501_mul(x, L, (uint64_t)n);
        }
        if (d2 < 65536) {
            a501_div(x, L, d1 * d2);
        } else {
            a501_div(x, L, d1);
            a501_div(x, L, d2);
        }
        /* add 1 */
        for (i = fw; i < L; i++) {
            x[i]++;
            if (x[i] != 0) break;
        }
    }

    for (i = L; i-- > fw;) {
        uint32_t limb = x[i];
        for (b = 3; b >= 0; b--) {
            r = (seq_t)(r << 8);
            r += (seq_t)((limb >> (8 * b)) & 255u);
        }
    }
    free(x);
    return r;
}

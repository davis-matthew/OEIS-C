/*
 * A005033  Number of nonequivalent dissections of a polygon into n
 *          quadrilaterals by nonintersecting diagonals rooted at a cell up to
 *          rotation.
 * https://oeis.org/A005033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ([z^m] G(z)^4 + [z^m] G(z^2)^2 + 2*[z^m] G(z^4)) / 4 with
 *            m = n-1, where G(z) = 1 + z*G(z)^3 (A001764) and [z^j] G(z)^k =
 *            k/(3j+k) * binomial(3j+k, j). Each coefficient is built exactly
 *            from its prime factorization in 32-bit limbs, then the sum is
 *            divided by 4 and converted to seq_t
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   a5033_add, a5033_coef, a5033_leg, a5033_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a5033_add.h"
#include "../utils/a5033_coef.h"
#include "../utils/a5033_leg.h"
#include "../utils/a5033_mul.h"

seq_t a(idx_t n) {
    uint64_t m, top, i, j;
    size_t cap, len, tlen;
    unsigned char *comp;
    uint32_t *d, *t;
    seq_t r = 0;
    if (n == 0) return 0;
    m = n - 1;
    top = 3 * m + 4;
    comp = (unsigned char *)calloc(top + 1, 1);
    for (i = 2; i * i <= top; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;
    cap = (size_t)((3 * m + 8) / 32 + 6);
    d = (uint32_t *)calloc(cap, sizeof(uint32_t));
    t = (uint32_t *)calloc(cap, sizeof(uint32_t));

    a5033_coef(d, &len, 4, m, comp);
    if (m % 2 == 0) {
        a5033_coef(t, &tlen, 2, m / 2, comp);
        a5033_add(d, &len, t, tlen);
    }
    if (m % 4 == 0) {
        a5033_coef(t, &tlen, 1, m / 4, comp);
        a5033_add(d, &len, t, tlen);
        a5033_add(d, &len, t, tlen);
    }
    /* divide by 4 */
    for (i = 0; i < len; i++) {
        uint32_t hi = (i + 1 < len) ? d[i + 1] : 0;
        d[i] = (d[i] >> 2) | (hi << 30);
    }
    while (len > 1 && d[len - 1] == 0) len--;

    for (i = len; i-- > 0;) {
        int s;
        for (s = 24; s >= 0; s -= 8) {
            r = r * 256 + (unsigned)((d[i] >> s) & 255u);
        }
    }
    free(comp);
    free(d);
    free(t);
    return r;
}

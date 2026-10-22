/*
 * A002216  Harary-Read numbers: restricted hexagonal polyominoes
 *          (cata-polyhexes) with n cells.
 * https://oeis.org/A002216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   with G(x) = sum g(n) x^n = (1-3x-sqrt(1-6x+5x^2))/(2x^2), i.e.
 *            g(0)=1, g(1)=3, (m+2)*g(m) = (6m+3)*g(m-1) - (5m-5)*g(m-2), the
 *            g.f. (1/(24x^2))*(12+24x-48x^2-24x^3+(1-x)^(3/2)(1-5x)^(3/2)
 *            -3(3+5x)sqrt(1-x^2)sqrt(1-5x^2)-4sqrt(1-x^3)sqrt(1-5x^3)) gives,
 *            for n>=2, 24*a(n) = 12*g(n-1) - 2*g(n) - 10*...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 62,
 *            big any n
 * Helpers:   hr_addmul, hr_divs, hr_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/hr_addmul.h"
#include "../utils/hr_divs.h"
#include "../utils/hr_sub.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)n;
    size_t L = (size_t)((n * 233) / 3200) + 6;
    uint32_t *buf = (uint32_t *)calloc(10 * L, sizeof(uint32_t));
    uint32_t *p0 = buf, *p1 = buf + L, *cur = buf + 2 * L;
    uint32_t *N0 = buf + 3 * L, *N1 = buf + 4 * L, *N2 = buf + 5 * L;
    uint32_t *S2 = buf + 6 * L, *S3 = buf + 7 * L;
    uint32_t *pos = buf + 8 * L, *neg = buf + 9 * L;

    idx_t i2 = (n % 2 == 0) ? n / 2 - 1 : (n - 3) / 2;
    int has3 = (n % 3 == 1);
    idx_t i3 = has3 ? (n - 4) / 3 : 0;

    for (idx_t m = 0; m <= n; m++) {
        memset(cur, 0, L * sizeof(uint32_t));
        if (m == 0) {
            cur[0] = 1;
        } else if (m == 1) {
            cur[0] = 3;
        } else {
            memset(neg, 0, L * sizeof(uint32_t));
            hr_addmul(cur, p1, L, (uint32_t)(6 * m + 3));
            hr_addmul(neg, p0, L, (uint32_t)(5 * m - 5));
            hr_sub(cur, neg, L);
            hr_divs(cur, L, (uint32_t)(m + 2));
        }
        if (m == i2) memcpy(S2, cur, L * sizeof(uint32_t));
        if (has3 && m == i3) memcpy(S3, cur, L * sizeof(uint32_t));
        if (m + 2 == n) memcpy(N2, cur, L * sizeof(uint32_t));
        if (m + 1 == n) memcpy(N1, cur, L * sizeof(uint32_t));
        if (m == n) memcpy(N0, cur, L * sizeof(uint32_t));
        uint32_t *t = p0; p0 = p1; p1 = cur; cur = t;
    }

    memset(pos, 0, L * sizeof(uint32_t));
    memset(neg, 0, L * sizeof(uint32_t));
    hr_addmul(pos, N1, L, 12);
    hr_addmul(pos, S2, L, (n % 2 == 0) ? 18 : 30);
    if (has3) hr_addmul(pos, S3, L, 8);
    hr_addmul(neg, N0, L, 2);
    hr_addmul(neg, N2, L, 10);
    hr_sub(pos, neg, L);
    hr_divs(pos, L, 24);

    size_t top = L;
    while (top > 0 && pos[top - 1] == 0) top--;
    seq_t r = 0;
    for (size_t i = top; i-- > 0;) {
        for (int b = 3; b >= 0; b--) {
            r = (seq_t)(r << 8);
            r += (seq_t)((pos[i] >> (8 * b)) & 0xFFu);
        }
    }
    free(buf);
    return r;
}

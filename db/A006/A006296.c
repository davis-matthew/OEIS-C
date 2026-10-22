/*
 * A006296  Number of genus 1 rooted maps with 3 faces with n vertices.
 * https://oeis.org/A006296
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x(1-sqrt(1-4x))(45+152x+(25+8x)sqrt(1-4x))/(2(1-4x)^(11/2))
 *            = x*[(10+122x+16x^2)(1-4x)^(-11/2) - (10+72x)(1-4x)^(-5)], terms
 *            from x^2. With c(k) = [x^k](1-4x)^(-11/2), c(0)=1, c(k) =
 *            c(k-1)*2*(2k+9)/k, and d(k) = [x^k](1-4x)^(-5) =
 *            4^k*binomial(k+4,4), K = n-3: a(n) = 10*c(K) + 122*c(K-1) +...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   a006296_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006296_gcd.h"

seq_t a(idx_t n) {
    idx_t K = n - 3;            /* n >= 4 so K >= 1 */
    seq_t c = 1, cK = 0, cK1 = 0, cK2 = 0;
    for (idx_t k = 0; k <= K; k++) {
        if (k > 0) {
            /* c = c * 2(2k+9) / k, exactly, without overflowing intermediates */
            uint64_t M = 2 * (2 * (uint64_t)k + 9);
            uint64_t r = (uint64_t)(c % (seq_t)k);
            uint64_t g = a006296_gcd(r, (uint64_t)k);
            c /= (seq_t)g;
            c *= (seq_t)(M / ((uint64_t)k / g));
        }
        if (K >= 2 && k == K - 2) cK2 = c;
        if (k == K - 1) cK1 = c;
        if (k == K) cK = c;
    }
    seq_t p = 1;                /* 4^(K-1) */
    for (idx_t i = 1; i < K; i++) p *= 4;
    uint64_t bK = (uint64_t)(K + 4) * (K + 3) / 2 * (K + 2) / 3 * (K + 1) / 4;
    uint64_t bK1 = (uint64_t)(K + 3) * (K + 2) / 2 * (K + 1) / 3 * K / 4;
    seq_t pos = cK * 10 + cK1 * 122 + cK2 * 16;
    seq_t neg = p * (seq_t)bK * 40 + p * (seq_t)bK1 * 72;
    return pos - neg;
}

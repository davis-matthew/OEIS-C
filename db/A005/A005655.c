/*
 * A005655  Number of board configurations in Mu Torere (for one player).
 * https://oeis.org/A005655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*A005654(n) + A005648(n), where A005654(n) = (A003239(n)
 *            + C(2*floor(n/2), floor(n/2)))/2 (bracelets), A003239(n) =
 *            (1/n)*Sum_{d|n} phi(n/d)*C(2d-1,d) (necklaces), and the outer
 *            term = C(2n-1,n) + C(n-1, floor((n-1)/2)); a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   tot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tot.h"

static uint64_t gcd_u(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* exact binomial; every intermediate is C(m, i) <= result */
static seq_t bin(idx_t m, idx_t k)
{
    seq_t r = 1;
    idx_t i;
    if (k > m) return (seq_t)0;
    if (k > m - k) k = m - k;
    for (i = 0; i < k; i++) {
        uint64_t den = (uint64_t)(i + 1);
        uint64_t g = gcd_u((uint64_t)(r % (seq_t)den), den);
        uint64_t num;
        if (g == 0) g = den;
        r /= (seq_t)g;
        den /= g;
        num = (uint64_t)(m - i) / den;
        r *= (seq_t)num;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t s, neck, brace, outer;
    idx_t d, m;
    if (n == 0) return (seq_t)1;
    s = 0;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            s += (seq_t)tot(n / d) * bin(2 * d - 1, d);
        }
    }
    neck = s / (seq_t)n;
    m = n / 2;
    brace = (neck + bin(2 * m, m)) / 2;
    outer = bin(2 * n - 1, n) + bin(n - 1, (n - 1) / 2);
    return brace + outer;
}

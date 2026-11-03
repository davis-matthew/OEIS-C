/*
 * A000150  Number of dissections of an n-gon, rooted at an exterior edge,
 *          asymmetric with respect to that edge.
 * https://oeis.org/A000150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(A000108(n)/2) - [n odd]*floor(A000108((n-1)/2)/2),
 *            i.e. a(2n) = A000108(2n)/2, a(2n+1) = (A000108(2n+1) -
 *            A000108(n))/2. Catalan numbers via C(k+1) = C(k)*2*(2k+1)/(k+2),
 *            with the small factors reduced by their gcd first so that no
 *            intermediate exceeds the result
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 197 (2026-11-01)
 * Helpers:   catalan, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/gcd_u64.h"

/* floor(C(n)/2) without forming C(n) itself */
static seq_t half_catalan(idx_t n)
{
    seq_t p, h;
    unsigned long long m, d, g;
    if (n == 0)
        return 0;
    p = catalan(n - 1);
    m = 2ULL * (2ULL * (unsigned long long)n - 1ULL);
    d = (unsigned long long)n + 1ULL;
    g = gcd_u64(m, d);
    m /= g;
    d /= g;
    p = p / (seq_t)d;            /* C(n) = p * m */
    if ((m & 1ULL) == 0) {
        h = p * (seq_t)(m / 2ULL);
    } else {
        h = p * (seq_t)((m - 1ULL) / 2ULL);
        h += p >> 1;
    }
    return h;
}

seq_t a(idx_t n)
{
    seq_t h = half_catalan(n);
    if (n & 1) {
        seq_t c = catalan((n - 1) / 2);
        h -= c >> 1;
    }
    return h;
}

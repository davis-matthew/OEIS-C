/*
 * A007318  Pascal's triangle read by rows: C(n,k) = binomial(n,k) =
 *          n!/(k!*(n-k)!), 0 <= k <= n.
 * https://oeis.org/A007318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy, core, look, hear, changed
 * Formula:   T(n,k) = C(n,k) = ((n-k+1)/k) * C(n,k-1), C(n,0) = 1; a(m) =
 *            T(r,k) with m = r*(r+1)/2 + k, 0 <= k <= r
 * Range:     u8 n <= 69, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 8841, big any n
 * Helpers:   a000037_isqrt, pascal_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/pascal_gcd.h"

seq_t a(idx_t n) {
    idx_t r = (a000037_isqrt(1 + 8 * n) - 1) / 2;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    while (r > 0 && r * (r + 1) / 2 > n)
        r--;

    idx_t k = n - r * (r + 1) / 2;
    if (k > r - k)
        k = r - k;

    seq_t result = 1;
    for (idx_t i = 0; i < k; i++) {
        seq_t den = (seq_t)(i + 1);
        if (den == 0)
            break; /* only reachable when the value cannot fit in seq_t */
        seq_t g = pascal_gcd(result, den);
        seq_t d = den / g;
        result = (result / g) * ((seq_t)(r - i) / d);
    }
    return result;
}

/*
 * A003168  Number of blobs with 2n+1 edges.
 * https://oeis.org/A003168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} binomial(n, k)*binomial(2*n+k, k-1)/n, a(0)
 *            = 1 (big integers). Fixed-width builds use the equivalent
 *            division-free g.f. relation P = 1 + z*(2*P^3 - P^2), P(z) = Sum
 *            a(n) z^n, i.e. a(n) = [z^(n-1)](2*P^3 - P^2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   a003168_big, a003168_fixed, a003168_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a003168_big.h"
#include "../utils/a003168_fixed.h"
#include "../utils/a003168_is_fixed.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (a003168_is_fixed()) return a003168_fixed(n);
    return a003168_big(n);
}

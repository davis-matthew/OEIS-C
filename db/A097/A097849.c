/*
 * A097849  Maximal entry in row n of A097847.
 * https://oeis.org/A097849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1<=k<=n} T(n,k), where T(n,k) = A097847(n,k) is the
 *            minimal number of distinct unit fractions summing to k/n (exact
 *            iterative-deepening search)
 * Range:     u8 n <= 18, u16 n <= 66, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Can a/b be written as a sum of at most m distinct unit fractions with denominators >= mind? */
static int A097849_search(seq_t a, seq_t b, int m, seq_t mind)
{
    if (a == 0) return 1;
    if (m <= 0) return 0;
    if (m == 1) {
        if (b % a != 0) return 0;
        return (b / a) >= mind;
    }
    seq_t lo = (b + a - 1) / a;
    if (lo < mind) lo = mind;
    seq_t hi = (seq_t)m * b / a;
    for (seq_t d = lo; d <= hi; ++d) {
        seq_t na = a * d - b;
        seq_t nb = b * d;
        if (na == 0) return 1;
        seq_t g = gcd(na, nb);
        na /= g;
        nb /= g;
        if (A097849_search(na, nb, m - 1, d + 1)) return 1;
    }
    return 0;
}

static int A097849_T(seq_t k, seq_t n)
{
    seq_t g = gcd(k, n);
    seq_t a = k / g, b = n / g;
    for (int m = 1;; ++m) {
        if (A097849_search(a, b, m, 2 > 1 ? (seq_t)1 : (seq_t)1)) return m;
    }
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    int best = 1;
    for (seq_t k = 1; k <= nn; ++k) {
        int t = A097849_T(k, nn);
        if (t > best) best = t;
    }
    return (seq_t)best;
}

/*
 * A216621  Triangle read by rows, n >= 1, 1 <= k <= n, T(n,k) = Sum_{c|n,d|k}
 *          phi(gcd(c,d)).
 * https://oeis.org/A216621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row and k the column in the lower
 *            triangular array read by rows, and T(r,k) = Sum_{c|r , d|k}
 *            phi(gcd(c,d)) (phi is Euler’s totient function)
 * Range:     u8 n <= 1829, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 93 terms through n = 9861 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

static void
divisors(idx_t n, idx_t *out, size_t *cnt)
{
    size_t c = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            out[c++] = d;
            if (d != n / d) out[c++] = n / d;
        }
    }
    *cnt = c;
}

static seq_t
triangle_value(idx_t r, idx_t k)
{
    seq_t sum = 0;
    idx_t divr[256], divk[256];
    size_t sr, sk;
    divisors(r, divr, &sr);
    divisors(k, divk, &sk);

    for (size_t i = 0; i < sr; ++i) {
        seq_t c = (seq_t)divr[i];
        for (size_t j = 0; j < sk; ++j) {
            seq_t d = (seq_t)divk[j];
            seq_t g = gcd(c, d);
            sum += totient((idx_t)g);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for n >= 1 only */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;   /* find smallest r with r(r+1)/2 >= n */
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;
    return triangle_value(r, k);
}

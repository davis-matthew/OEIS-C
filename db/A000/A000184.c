/*
 * A000184  Number of genus 0 rooted maps with 3 faces with n vertices.
 * https://oeis.org/A000184
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2n+1)*binomial(2n,n)/6 - n*4^(n-1), computed with T(1)
 *            = 1, T(n) = T(n-1)*2*(2n+1)/(n-1) = n*(2n+1)*binomial(2n,n)/6,
 *            so a(n) = T(n) - n*4^(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   a184_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a184_g.h"

seq_t a(idx_t n)
{
    seq_t T = 1;
    seq_t p = 1;
    idx_t k;
    for (k = 2; k <= n; k++) {
        unsigned long long m = 2ULL * (2ULL * (unsigned long long)k + 1ULL);
        unsigned long long d = (unsigned long long)(k - 1);
        unsigned long long g = a184_g(m, d);
        m /= g;
        d /= g;
        T /= (seq_t)d;
        T *= (seq_t)m;
    }
    for (k = 1; k < n; k++)
        p *= 4;
    return T - (seq_t)n * p;
}

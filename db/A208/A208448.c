/*
 * A208448  Greatest common divisors of consecutive floor-factorial numbers
 *          (A010786).
 * https://oeis.org/A208448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD(A010786(n+1), A010786(n)), A010786(m) =
 *            Product_{k=1..m} floor(m/k); computed via prime exponents (min
 *            of exponents)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 35, u64 n <= 65, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 979 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static void A208448_fill(idx_t m, unsigned long long *e)
{
    idx_t k = 1;
    while (k <= m) {
        idx_t v = m / k;
        idx_t next_k = m / v + 1;
        unsigned long long cnt = (unsigned long long)(next_k - k);
        idx_t x = v;
        for (idx_t p = 2; p * p <= x; p++) {
            if (x % p == 0) {
                unsigned long long c = 0;
                while (x % p == 0) { x /= p; c++; }
                e[p] += c * cnt;
            }
        }
        if (x > 1) e[x] += cnt;
        k = next_k;
    }
}

seq_t a(idx_t n)
{
    size_t sz = (size_t)n + 2;
    unsigned long long *e1 = (unsigned long long *)calloc(sz, sizeof(unsigned long long));
    unsigned long long *e2 = (unsigned long long *)calloc(sz, sizeof(unsigned long long));
    seq_t ans = (seq_t)1;
    if (!e1 || !e2) { free(e1); free(e2); return ans; }
    A208448_fill(n, e1);
    A208448_fill(n + 1, e2);
    for (idx_t p = 2; p <= n + 1; p++) {
        unsigned long long mn = e1[p] < e2[p] ? e1[p] : e2[p];
        for (unsigned long long i = 0; i < mn; i++)
            ans *= (seq_t)p;
    }
    free(e1);
    free(e2);
    return ans;
}

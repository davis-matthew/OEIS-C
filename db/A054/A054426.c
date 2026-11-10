/*
 * A054426  Inverse permutation to A054424.
 * https://oeis.org/A054426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(j) = nthmember(j, A054424): decode heap index j to the
 *            fraction p/q by its Stern-Brocot path, then a(j) =
 *            (sum_{t=2}^{p+q-1} phi(t)) + #{x in 1..p : gcd(x,p+q)=1}
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A054426_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t p = 1, q = 1;
    for (int i = 0; (n >> i) > 1; ++i) {
        if (((n >> i) & 1) == 0) q += p;   /* left child */
        else p += q;                        /* right child */
    }
    uint64_t s = p + q;

    /* sum of phi(t) for t = 2 .. s-1 via linear sieve */
    seq_t total = 0;
    if (s > 2) {
        uint64_t lim = s - 1;
        uint32_t *phi = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
        uint32_t *primes = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
        size_t pc = 0;
        phi[1] = 1;
        for (uint64_t i = 2; i <= lim; ++i) {
            if (phi[i] == 0) { phi[i] = (uint32_t)(i - 1); primes[pc++] = (uint32_t)i; }
            for (size_t k = 0; k < pc; ++k) {
                uint64_t v = (uint64_t)primes[k] * i;
                if (v > lim) break;
                if (i % primes[k] == 0) { phi[v] = phi[i] * primes[k]; break; }
                phi[v] = phi[i] * (primes[k] - 1);
            }
            total += (seq_t)phi[i];
        }
        free(phi);
        free(primes);
    }
    for (uint64_t x = 1; x <= p; ++x)
        if (A054426_gcd(x, s) == 1) ++total;
    return total;
}

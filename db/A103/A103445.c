/*
 * A103445  Sum of the numbers of unitary divisors of the binomial
 *          coefficients C(n,k), k=0..n.
 * https://oeis.org/A103445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} 2^{ω(C(n,k))} where ω(m) is the number of
 *            distinct prime divisors of m. For each binomial coefficient
 *            C(n,k) the number of unitary divisors equals 2^{ω(C(n,k))}. The
 *            function computes the row sum of this triangle
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow2_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2_uint.h"

seq_t a(idx_t n)
{
    /* n = 0: only C(0,0)=1 has ω = 0 → 2^0 = 1 */
    if (n == 0)
        return (seq_t)1;

    /* ---------- generate primes ≤ n via sieve ---------- */
    idx_t size = n + 1;
    bool *is_comp = (bool *)calloc((size_t)size, sizeof(bool));
    if (!is_comp)
        return (seq_t)0;          /* failure – unlikely */
    idx_t *primes = (idx_t *)malloc((size_t)size * sizeof(idx_t));
    if (!primes) {
        free(is_comp);
        return (seq_t)0;
    }

    idx_t pcnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[pcnt++] = i;
            for (idx_t j = i + i; j <= n; j += i)
                is_comp[j] = true;
        }
    }
    free(is_comp);

    /* if there are no primes (n = 1), all binomials are 1
       and the sum equals n+1.  We also free the primes array. */
    if (pcnt == 0) {
        free(primes);
        return (seq_t)(n + 1);
    }

    /* ---------- precompute v_p(i!) for all i ≤ n and for each prime p ---------- */
    /* v[p][i] = exponent of prime p in i! */
    size_t ventries = (size_t)pcnt * (size_t)(n + 1);
    uint16_t *v = (uint16_t *)malloc(ventries * sizeof(uint16_t));
    if (!v) {
        free(primes);
        return (seq_t)0;
    }

    for (idx_t pi = 0; pi < pcnt; ++pi) {
        uint16_t *vp = v + (size_t)pi * (size_t)(n + 1);
        vp[0] = 0;
        idx_t p = primes[pi];
        for (idx_t i = 1; i <= n; ++i) {
            int e = 0;
            idx_t t = i;
            while (t % p == 0) {
                ++e;
                t /= p;
            }
            vp[i] = vp[i - 1] + (uint16_t)e;
        }
    }

    /* ---------- compute the row sum: Σ 2^{ω(C(n,k))} ---------- */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t cnt = 0;   /* number of primes with positive exponent in C(n,k) */
        for (idx_t pi = 0; pi < pcnt; ++pi) {
            uint16_t *vp = v + (size_t)pi * (size_t)(n + 1);
            int e = (int)vp[n] - (int)vp[k] - (int)vp[n - k];
            if (e > 0)
                ++cnt;
        }
        sum += pow2_uint(cnt);
    }

    free(v);
    free(primes);

    return sum;
}

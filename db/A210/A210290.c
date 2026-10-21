/*
 * A210290  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          nonnegative determinant.
 * https://oeis.org/A210290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^4 + Z(n)) / 2 * where Z(n) is the number of 2x2
 *            matrices with entries in {0,…,n} * that have determinant zero. *
 *            The number of zero‑determinant matrices is * Z(n) = 2*(n+1)^2 –
 *            1 + Σ_{m=1..n} S_m * ⌊n/m⌋^2 * with S_1 = 3 and S_m = 2*φ(m) (φ
 *            = Euler’s totient) for m>1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 302, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *phi_cache = NULL;
static idx_t phi_cache_size = 0;

/* Recompute totient values up to n (inclusive) */
static void recompute_phi_upto(idx_t n)
{
    /* Allocate new array */
    uint64_t *phi = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!phi) return; /* Out of memory – accept failure; caller will crash */
    for (idx_t i = 0; i <= n; ++i) phi[i] = i;
    phi[0] = 0;
    if (n >= 1) phi[1] = 1;
    for (idx_t p = 2; p <= n; ++p)
    {
        if (phi[p] == p)          /* p is prime */
        {
            for (idx_t j = p; j <= n; j += p)
                phi[j] -= phi[j] / p;
        }
    }
    free(phi_cache);
    phi_cache = phi;
    phi_cache_size = n + 1;
}

seq_t a(idx_t n)
{
    /* Ensure totient values up to n are available */
    if (phi_cache_size <= n)
        recompute_phi_upto(n);
    /* Compute (n+1)^4 */
    seq_t np1 = ((seq_t)n) + 1;
    seq_t np1_sq = np1 * np1;
    seq_t total = np1_sq * np1_sq;

    /* Sum S_m * floor(n/m)^2 */
    seq_t sumS = 0;
    for (idx_t m = 1; m <= n; ++m)
    {
        seq_t t = ((seq_t)n) / ((seq_t)m);
        seq_t t2 = t * t;
        seq_t coeff;
        if (m == 1)
            coeff = 3;
        else
            coeff = (seq_t)(2 * phi_cache[m]); /* phi_cache uses uint64_t */
        sumS += coeff * t2;
    }

    /* Zero determinant count */
    seq_t z = (seq_t)2 * np1_sq - 1 + sumS;

    /* Result */
    return (total + z) / 2;
}

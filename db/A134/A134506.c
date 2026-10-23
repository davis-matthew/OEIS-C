/*
 * A134506  Number of 2 X 2 singular integer matrices with elements from
 *          {1,...,n}.
 * https://oeis.org/A134506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Σ_{m=1..n} pairs(m) * ⌊n/m⌋² * where pairs(m) = 1 for m =
 *            1, and 2·φ(m) for m ≥ 2, * and φ(m) is Euler’s totient function.
 *            * This counts all 2×2 singular integer matrices with entries in
 *            {1,…,n}
 * Range:     u8 n <= 9, u16 n <= 111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *phi_cache = NULL;
static idx_t phi_cap = 0;

/* Recompute Euler's totients up to n (inclusive).  */
static void compute_totients(idx_t n)
{
    if (n + 1 > phi_cap) {
        uint32_t *new_phi = (uint32_t *)realloc(phi_cache, (n + 1) * sizeof(uint32_t));
        if (!new_phi)
            return; /* Allocation failed; caller will get 0. */
        phi_cache = new_phi;
        phi_cap = n + 1;
    }

    for (idx_t i = 0; i <= n; ++i)
        phi_cache[i] = (uint32_t)i;

    for (idx_t p = 2; p <= n; ++p) {
        if (phi_cache[p] == p) {          /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi_cache[j] -= phi_cache[j] / (uint32_t)p;
        }
    }
    /* phi[0] is unused; phi[1] remains 1. */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    if (n == 0)
        return 0;

    compute_totients(n);

    for (idx_t m = 1; m <= n; ++m) {
        idx_t pairs = (m == 1) ? 1 : (2 * (idx_t)phi_cache[m]);
        idx_t t = n / m;
        seq_t add = (seq_t)pairs * (seq_t)t * (seq_t)t;
        sum += add;
    }

    return sum;
}

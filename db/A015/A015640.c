/*
 * A015640  Number of 5-tuples of different integers from [ 1,n ] with no
 *          global factor.
 * https://oeis.org/A015640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1}^{n} μ(d) * C(⌊n/d⌋, 5) (μ = Möbius function)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* compute Möbius function values up to limit */
static void compute_mu(idx_t limit, int8_t *mu)
{
    /* mark composites; 0=unmarked, 1=marked */
    uint8_t *is_comp = (uint8_t*)calloc((size_t)(limit + 1), 1);
    if (!is_comp) return;                  /* allocation failed; caller will not use mu */

    /* store primes found; capacity limit+1 is safe */
    uint32_t *primes = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    size_t prcnt = 0;
    mu[0] = 0;
    mu[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            primes[prcnt++] = (uint32_t)i;
            mu[(size_t)i] = -1;
        }
        for (size_t j = 0; j < prcnt; ++j) {
            uint32_t p = primes[j];
            uint64_t t = (uint64_t)i * p;
            if (t > (uint64_t)limit) break;
            is_comp[(size_t)t] = 1;
            if (i % p == 0) {
                mu[(size_t)t] = 0;
                break;
            } else {
                mu[(size_t)t] = -mu[(size_t)i];
            }
        }
    }

    free(is_comp);
    free(primes);
}

seq_t a(idx_t n)
{
    /* For n < 5 there can be no 5‑tuple at all */
    if (n < 5) return (seq_t)0;

    /* Allocate Möbius array and compute it */
    int8_t *mu = (int8_t*)malloc((size_t)(n + 1) * sizeof(int8_t));
    if (!mu) return (seq_t)0;               /* allocation failed; return 0 as a safe fallback */
    compute_mu(n, mu);

    seq_t result = 0;

    /* Min product constant 120 = 5! */
    const seq_t denom = (seq_t)120;

    for (idx_t d = 1; d <= n; ++d) {
        idx_t m = n / d;
        if (m < 5) continue;               /* C(m,5) = 0 when m < 5 */

        /* Compute C(m,5) = m*(m-1)*(m-2)*(m-3)*(m-4)/120 */
        seq_t m_seq = (seq_t)m;
        seq_t binom = m_seq * (m_seq - 1) * (m_seq - 2) * (m_seq - 3) * (m_seq - 4) / denom;

        if (mu[d] == 1)   result += binom;
        else if (mu[d] == -1) result -= binom;
        /* mu[d]==0 contributes nothing */
    }

    free(mu);
    return result;
}

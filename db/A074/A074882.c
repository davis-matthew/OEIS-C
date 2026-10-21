/*
 * A074882  Number of integers in {1, 2, ..., sigma(n)} that are coprime to n.
 * https://oeis.org/A074882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * floor(σ(n)/d) (σ = sum of divisors)
 * Range:     u8 n <= 16, u16 n <= 522, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_uint64.h"

static seq_t compute_sigma_uint64(uint64_t n)
{
    uint64_t primes[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(n, primes, exp, &cnt);

    seq_t sigma = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exp[i];

        /* Compute p^{e+1} */
        seq_t pe = 1;
        for (int j = 0; j < e + 1; ++j) {
            pe *= (seq_t)p;
        }

        /* (p^{e+1} - 1) / (p - 1) */
        seq_t num = pe - (seq_t)1;
        seq_t den = (seq_t)(p - 1);
        sigma *= num / den;
    }
    return sigma;
}

/* Count of integers in {1,2,…,σ(n)} that are coprime to n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1, this case is unused */

    uint64_t x = (uint64_t)n;
    uint64_t primes[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(x, primes, exp, &cnt);

    seq_t sigma = compute_sigma_uint64(x);
    int64_t sum = 0;

    /* Iterate over all subsets of the distinct prime factors */
    for (int mask = 0; mask < (1 << cnt); ++mask) {
        seq_t prod = 1;
        int bits = 0;
        for (int j = 0; j < cnt; ++j) {
            if (mask & (1 << j)) {
                prod *= (seq_t)primes[j];
                ++bits;
            }
        }
        seq_t term = sigma / prod;
        if ((bits & 1) == 0)
            sum += (int64_t)term;
        else
            sum -= (int64_t)term;
    }

    return (seq_t)sum;
}

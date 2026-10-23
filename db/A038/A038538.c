/*
 * A038538  Number of semisimple rings with n elements.
 * https://oeis.org/A038538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, mult
 * Formula:   a(n) = ∏_{p^k‖n} A004101(k) (multiplicative), where A004101(k)
 *            counts unordered sums of terms m·n² with m,n≥1
 * Range:     u8 n <= 8159, u16 n <= 8159, u32 n <= 8159, u64 n <= 8159,
 *            u128 n <= 8159, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

#define MAX_K 64          /* maximum exponent we consider (≥ log₂(UINT64_MAX)) */
static seq_t ways[MAX_K + 1];
static bool ways_initialized = false;

static void init_ways(void)
{
    /* dynamic programming to compute A004101(k) for 0 ≤ k ≤ MAX_K */
    for (int i = 0; i <= MAX_K; ++i) ways[i] = 0;
    ways[0] = 1;

    for (int n = 1; n * n <= MAX_K; ++n) {
        int nsq = n * n;
        for (int m = 1; m * nsq <= MAX_K; ++m) {
            int w = m * nsq;                    /* weight of this component type */
            for (int s = w; s <= MAX_K; ++s) {
                ways[s] += ways[s - w];
            }
        }
    }
    ways_initialized = true;
}

seq_t a(idx_t n)
{
    if (!ways_initialized)
        init_ways();

    uint64_t nn = (uint64_t)n;
    if (nn <= 1)            /* a(1) = 1 */
        return 1;

    seq_t result = 1;

    /* factorise nn into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t k = exps[i];
        result *= ways[(size_t)k];
    }
    return result;
}

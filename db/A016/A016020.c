/*
 * A016020  Least k such that (tau(k^4)+3)/4=n.
 * https://oeis.org/A016020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that (tau(k^4)+3)/4 = n+1, i.e. tau(k^4) =
 *            4*n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"

static const uint64_t SMALL_PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113
};
static const size_t SMALL_PRIMES_CNT = sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]);

static void process_factorisation(const uint64_t *factors, int flen, seq_t *best)
{
    /* Convert factors 4*q+1 to exponents q. */
    int exps[20];
    for (int i = 0; i < flen; ++i) exps[i] = (int)((factors[i] - 1) / 4);

    /* Sort exponents in non‑increasing order (descending). */
    for (int i = 0; i < flen - 1; ++i)
        for (int j = i + 1; j < flen; ++j)
            if (exps[i] < exps[j]) {
                int t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }

    seq_t k = 1;
    for (int i = 0; i < flen; ++i)
        k *= pow_seq((seq_t)SMALL_PRIMES[i], exps[i]);

    if (*best == 0 || k < *best) *best = k;
}

static void dfs(uint64_t rem, uint64_t start, uint64_t *factors, int depth, seq_t *best)
{
    if (rem == 1) {
        if (depth > 0)
            process_factorisation(factors, depth, best);
        return;
    }

    for (uint64_t d = start; d * d <= rem; ++d) {
        if (rem % d == 0 && d % 4 == 1) {
            factors[depth] = d;
            dfs(rem / d, d, factors, depth + 1, best);
        }
    }

    if (rem >= start && rem % 4 == 1 && rem != 1) {
        factors[depth] = rem;
        dfs(1, rem, factors, depth + 1, best);
    }
}

seq_t a(idx_t n)
{
    /* m = 4*n + 1 = tau(k^4). */
    uint64_t m = (uint64_t)4 * (uint64_t)n + 1;

    if (m == 1)  /* corresponds to n = 0 */
        return 1;

    seq_t best = 0;
    uint64_t buf[20];
    dfs(m, 5, buf, 0, &best);
    return best;
}

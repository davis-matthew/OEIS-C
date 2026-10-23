/*
 * A181951  Number of cyclic subgroups of prime order in the Alternating Group
 *          A_n.
 * https://oeis.org/A181951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A186202(n) - A001465(n) where A186202(n) counts cyclic
 *            subgroups of prime order in S_n and A001465(n) is the number of
 *            primes ≤ n. This implementation uses the combinatorial formulas:
 *            a(n) = sum_{p odd prime ≤ n} 1/(p-1) * ∑_{k=1}^{⌊n/p⌋} n!/(p^k
 *            k! (n-kp)!) + ∑_{k even ≥2}^{⌊n/2⌋} n!/(2^k...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   binom, build_primes, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* store a static list of primes up to the largest n seen so far */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_max = 0;

    if (n > primes_max) {
        free(primes);
        build_primes(n, &primes, &primes_cnt);
        primes_max = n;
    }

    seq_t ans = 0;

    /* odd primes: contribution of cyclic subgroups of order p */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;
        if (p == 2) continue;          /* p = 2 handled later */

        idx_t kmax = n / p;
        if (!kmax) continue;

        seq_t fact_p_minus2 = factorial(p - 2);   /* (p-2)! */
        seq_t fact_p_minus1 = factorial(p - 1);   /* (p-1)! */

        /* k = 1 term: binom(n,p) * (p-2)! */
        seq_t S = binom(n, p) * fact_p_minus2;
        ans += S;

        /* k ≥ 2 terms */
        for (idx_t k = 2; k <= kmax; ++k) {
            seq_t bin = binom(n - (k - 1) * p, p);
            /* S = S * bin * (p-1)! / k */
            S = S * bin;
            S = S * fact_p_minus1;
            S = S / (seq_t)k;
            ans += S;
        }
    }

    /* even involutions (subgroups of order 2 contained in A_n) */
    idx_t kmax2 = n / 2;
    seq_t term = 1;   /* term_k for current k, starting with k = 0 */
    for (idx_t k = 1; k <= kmax2; ++k) {
        seq_t bin = binom(n - 2 * (k - 1), 2);
        term = term * bin;
        term = term / (seq_t)k;           /* exact division */
        if ((k & 1) == 0)                /* only even k */
            ans += term;
    }

    return ans;
}

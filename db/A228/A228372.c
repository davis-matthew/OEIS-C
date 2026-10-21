/*
 * A228372  Number of nontrivial divisors in the first n composites.
 * https://oeis.org/A228372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ( τ(A002808(k)) – 2 ), where τ(m) is the
 *            number of divisors of the composite m. This is the cumulative
 *            total of non‑trivial divisors of the first n composites
 * Range:     u8 n <= 68, u16 n <= 7901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    static idx_t cached_n = 0;          /* how many terms have been computed */
    static seq_t *cum = NULL;           /* cumulative sums   a(1)…a(cached_n) */
    static size_t cum_cap = 0;          /* allocated capacity for cum */
    static idx_t cur_num = 3;           /* last number examined (start at 3) */

    if (n == 0) return 0;              /* undefined for n=0, return 0 */

    /* initialize storage */
    if (cum_cap == 0) {
        cum_cap = 256;
        cum = (seq_t *)malloc(cum_cap * sizeof(seq_t));
    }

    /* extend precomputation up to n */
    while (cached_n < n) {
        /* find next composite number */
        do { cur_num++; } while (is_prime(cur_num));

        /* factor cur_num to compute τ(cur_num) */
        uint64_t m = (uint64_t)cur_num;
        uint64_t primes[64], exps[64];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        seq_t tau = 1;
        for (size_t i = 0; i < cnt; ++i) {
            tau *= (seq_t)(exps[i] + 1);
        }

        seq_t nontriv = tau - 2;                /* τ – 2 */

        /* grow cumulative array if needed */
        if (cached_n >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            seq_t *new_arr = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
            if (!new_arr) exit(EXIT_FAILURE);   /* allocation failed */
            cum = new_arr;
            cum_cap = new_cap;
        }

        /* store cumulative sum */
        if (cached_n == 0) {
            cum[0] = nontriv;
        } else {
            cum[cached_n] = cum[cached_n - 1] + nontriv;
        }

        cached_n++;   /* we have now computed a(cached_n) */
    }

    return cum[n - 1];
}

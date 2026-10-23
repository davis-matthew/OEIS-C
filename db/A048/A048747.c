/*
 * A048747  Factor composite n, divide sum of divisors by number of divisors;
 *          append n to sequence if quotient is integral and not previously
 *          seen.
 * https://oeis.org/A048747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite integer whose mean divisor is an integer
 *            and has not been seen before
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, grow_seq, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/grow_seq.h"

static void compute_sigma_d(idx_t n, seq_t *sigma, seq_t *dd)
{
    uint64_t tmp = (uint64_t)n;
    size_t cnt;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(tmp, primes, exps, &cnt);

    seq_t s = 1;
    seq_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t p_pow = seq_pow(p, e + 1);          /* p^(e+1) */
        seq_t term = (p_pow - (seq_t)1) / (p - (seq_t)1); /* (p^(e+1)-1)/(p-1) */
        s = s * term;
        d = d * ((seq_t)(e + 1));
    }
    *sigma = s;
    *dd = d;
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static seq_t *means = NULL;
    static size_t means_cap = 0;
    static size_t means_len = 0;
    static idx_t next_candidate = 4;   /* first composite after 3 */

    if (!init) {
        grow_seq(&terms, &terms_cap, 16);
        grow_seq(&means, &means_cap, 16);
        init = true;
    }

    while (terms_len < n) {
        seq_t sigma, d;
        compute_sigma_d(next_candidate, &sigma, &d);

        /* skip primes (d == 2) */
        if (d > (seq_t)2 && sigma % d == (seq_t)0) {
            seq_t mean = sigma / d;
            bool seen = false;
            for (size_t i = 0; i < means_len; ++i) {
                if (means[i] == mean) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                if (terms_len >= terms_cap) {
                    grow_seq(&terms, &terms_cap, terms_len + 1);
                }
                terms[terms_len] = (seq_t)next_candidate;
                ++terms_len;

                if (means_len >= means_cap) {
                    grow_seq(&means, &means_cap, means_len + 1);
                }
                means[means_len] = mean;
                ++means_len;
            }
        }
        ++next_candidate;
    }

    return terms[(size_t)n - 1];
}

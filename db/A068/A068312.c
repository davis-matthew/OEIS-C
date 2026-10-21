/*
 * A068312  Arithmetic derivative of triangular numbers.
 * https://oeis.org/A068312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n*(n+1)/2), where d is the arithmetic derivative
 * Range:     u8 n <= 22, u16 n <= 223, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor.h"

seq_t a(idx_t n)
{
    /* base cases */
    if (n <= 1)
        return 0;

    /* triangular number */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* factor n and n+1 */
    uint64_t pr_n[64], pr_np1[64];
    uint64_t exp_n[64], exp_np1[64];
    size_t cnt_n, cnt_np1;

    factor((uint64_t)n, pr_n, exp_n, &cnt_n);
    factor((uint64_t)(n + 1), pr_np1, exp_np1, &cnt_np1);

    /* merge the factorizations (including the exponent of 2 from both numbers) */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t total = 0;

    for (size_t i = 0; i < cnt_n; i++) {
        primes[total] = pr_n[i];
        exps[total]   = exp_n[i];
        total++;
    }

    for (size_t i = 0; i < cnt_np1; i++) {
        size_t j;
        for (j = 0; j < total; j++) {
            if (primes[j] == pr_np1[i]) {
                exps[j] += exp_np1[i];
                break;
            }
        }
        if (j == total) {
            primes[total] = pr_np1[i];
            exps[total]   = exp_np1[i];
            total++;
        }
    }

    /* divide by 2 once (since T = n*(n+1)/2) */
    for (size_t i = 0; i < total; i++) {
        if (primes[i] == 2) {
            if (exps[i] > 0)
                exps[i]--;          /* subtract one exponent of 2 */
            break;
        }
    }

    /* compute the arithmetic derivative: sum e_p * T/p over all primes p of T */
    seq_t result = 0;
    for (size_t i = 0; i < total; i++) {
        uint64_t p = primes[i];
        size_t e   = exps[i];
        if (e == 0)
            continue;

        seq_t part = T / (seq_t)p;
        part *= (seq_t)e;
        result += part;
    }

    return result;
}

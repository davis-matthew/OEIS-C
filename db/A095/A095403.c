/*
 * A095403  Sum of digits of n minus the sum of digits of all distinct prime
 *          factors of n.
 * https://oeis.org/A095403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign, changed
 * Formula:   a(n) = digit_sum(n) - Σ digit_sum(p) for all distinct prime
 *            factors p of n
 * Range:     i8 n <= 702, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 127 terms through n = 8734 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* --------------------------------------------------------------- */
static void factor(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; exp[c] = 0;
            while (n % p == 0) { n /= p; ++exp[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* Sum of digits of n */
    seq_t sum_n = digit_sum((seq_t)n);

    /* Factorize n and accumulate digit sums of distinct prime factors */
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor((uint64_t)n, primes, exps, &cnt);

    seq_t sum_pf = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum_pf += digit_sum((seq_t)primes[i]);
    }

    /* Difference */
    return sum_n - sum_pf;
}

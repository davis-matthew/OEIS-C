/*
 * A138503  a(n) = Sum_{d|n} (-1)^(d-1)*d^3.
 * https://oeis.org/A138503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * d^3 (OEIS A138503)
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 1223, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursively enumerate the divisors of n from its prime powers.
   `primes` and `exps` contain the factorisation of n,
   `cnt`   the number of distinct prime factors,
   `idx`   the current factor being processed,
   `cur`   the product of the chosen powers so far,
   `sum`   pointer to the running total. */
static void dfs(const uint64_t *primes, const uint64_t *exps,
                size_t cnt, size_t idx, seq_t cur, seq_t *sum)
{
    if (idx == cnt) {
        /* d = cur */
        seq_t sign = (cur & 1) ? 1 : -1;   /* (-1)^(d-1) */
        seq_t d3   = cur * cur * cur;      /* d^3 */
        *sum += sign * d3;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t exp = exps[idx];

    seq_t product = cur;
    for (size_t i = 0; i <= exp; ++i) {
        dfs(primes, exps, cnt, idx + 1, product, sum);
        product *= (seq_t)p;
    }
}

/* Public sequence entry */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    /* For n >= 1 is defined by the formula. */

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(primes, exps, cnt, 0, (seq_t)1, &sum);
    return sum;
}

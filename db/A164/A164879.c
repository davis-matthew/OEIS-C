/*
 * A164879  Maximum number of copies of distinct prime divisors of n required
 *          to express n as a sum; a(1) = 0 by convention.
 * https://oeis.org/A164879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   search_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimal t such that n can be written as Σ_{p|n} c_p·p with 0 ≤ c_p ≤ t */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/search_rec.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                      /* a(1) = 0 by convention */

    uint64_t nn = (uint64_t)n;                 /* work in 64‑bit */
    uint64_t primes[10];
    size_t r = 0;
    uint64_t temp = nn;

    /* factor to distinct primes */
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            primes[r++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) primes[r++] = temp;          /* last prime factor */

    if (r == 0) return 0;                      /* n = 0, not expected but safe */
    if (r == 1) return (seq_t)(nn / primes[0]);/* prime power: a(p^k)=p^(k-1) */

    /* sum of primes and suffix sums for pruning */
    uint64_t sum_pr = 0;
    for (size_t i = 0; i < r; ++i) sum_pr += primes[i];

    uint64_t sum_rem[11];
    sum_rem[r] = 0;
    for (int i = (int)r - 1; i >= 0; --i) sum_rem[i] = sum_rem[i + 1] + primes[i];

    /* lower bound from ceiling(n / sum of distinct primes) */
    uint64_t t_low = (nn + sum_pr - 1) / sum_pr;

    /* increase t until feasible */
    for (uint64_t t = t_low; ; ++t) {
        if (search_rec(0, nn, t, primes, r, sum_rem))
            return (seq_t)t;
    }
}

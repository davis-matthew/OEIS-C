/*
 * A230593  a(n) = n * Sum_{q|n} 1 / q, where q are noncomposite numbers
 *          (A008578) dividing n.
 * https://oeis.org/A230593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{p|n} n/p, where the sum is over distinct primes p
 *            dividing n (i.e. a(n) = n + the sum of n divided by each
 *            distinct prime factor of n)
 * Range:     u8 n <= 137, u16 n <= 9953, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* n is always ≥1 in the sequence, but guard anyway */

    seq_t sum = 0;
    if (n > 1) {
        /* n is small (≤~10000 in expected usage), so a fixed‑size array is safe */
        uint64_t primes[32];
        uint64_t exps[32];
        size_t cnt = 0;
        factor_u64(n, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            /* primes[i] is a distinct prime factor of n */
            sum += (seq_t)(n / primes[i]);   /* n/p */
        }
    }
    return (seq_t)n + sum;   /* n + Σ n/p */
}

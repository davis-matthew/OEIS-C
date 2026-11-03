/*
 * A025540  Least common multiple of {C(0,0), C(2,1), ..., C(2n,n)}.
 * https://oeis.org/A025540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( C(2k,k) for 0 ≤ k ≤ n ) a(n) = 2^⌊log₂(n+1)⌋ · ∏_{p
 *            odd prime ≤ 2n} p^⌊log_p(2n)⌋
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 992 (2026-11-01)
 * Helpers:   build_primes_upto, ilog2, ipow_u64_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ilog2.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/pow2.h"

/* Compute the n-th term of sequence A025540 */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Upper bound for primes needed */
    idx_t limit = 2 * n;

    /* Generate all primes ≤ 2n */
    size_t prime_count;
    uint64_t *primes;
    build_primes_upto(limit, &primes, &prime_count);

    /* Exponent of 2: ⌊log₂(n+1)⌋ */
    idx_t exp2 = ilog2(n + 1);
    seq_t result = pow2(exp2);

    /* Multiply prime powers for all odd primes ≤ 2n */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p == 2) {
            continue;          /* already handled */
        }

        /* Compute ⌊log_p(2n)⌋ */
        idx_t exp = 0;
        uint64_t power = p;
        while (power <= limit) {
            ++exp;
            power *= p;
        }

        /* Multiply by p^exp */
        seq_t pp = ipow_u64_idx(p, exp);
        result = result * pp;
    }

    free(primes);
    return result;
}

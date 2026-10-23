/*
 * A137926  a(n) = the largest divisor of n that is coprime to A000005(n).
 *          (A000005(n) = the number of positive divisors of n.)
 * https://oeis.org/A137926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor d of n that is coprime to tau(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

/* helper: power of a uint64_t base raised to exp, stored in seq_t */
static seq_t pow_seq_uint64(uint64_t base, uint32_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    for (uint32_t i = 0; i < exp; ++i)
        result *= b;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for zero, but return 0 for safety */
        return (seq_t)0;

    uint64_t n_u = (uint64_t)n;

    /* tau(n) = number of divisors of n */
    seq_t tau_seq = divisor_count_u64(n_u);
    uint64_t tau = (uint64_t)tau_seq;   /* tau fits in 64 bits */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n_u, primes, exps, &cnt);

    /* build the largest divisor with primes not dividing tau(n) */
    seq_t ans = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (tau % p != 0) {            /* p does not divide tau => include the whole power */
            seq_t term = pow_seq_uint64(p, (uint32_t)exps[i]);
            ans *= term;
        }
    }
    return ans;
}

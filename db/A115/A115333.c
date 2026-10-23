/*
 * A115333  Sum of primes that do not divide n and are less than the largest
 *          prime dividing n.
 * https://oeis.org/A115333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{p<lpf(n)} p – Σ_{q|n, q<p and q prime} q where lpf(n)
 *            is the largest prime divisor of n (A006530(n))
 * Range:     u8 n <= 46, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility headers supplied by the build system */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x) */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Factor n to obtain its prime divisors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Determine the largest prime divisor lpf. */
    uint64_t lpf = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] > lpf)
            lpf = primes[i];

    if (lpf <= 2)  /* no primes < 2 */
        return (seq_t)0;

    /* Sum all primes p with p < lpf.   */
    seq_t sum_all = 0;
    for (uint64_t p = 2; p < lpf; ++p)
        if (is_prime(p))
            sum_all += (seq_t)p;

    /* Subtract those primes that divide n.   */
    seq_t sum_div = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] < lpf)
            sum_div += (seq_t)primes[i];

    return sum_all - sum_div;
}

/*
 * A167447  Number of divisors of n which are not multiples of 3 consecutive
 *          primes.
 * https://oeis.org/A167447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not multiples * of any
 *            three consecutive primes. * * The implementation factorizes n,
 *            splits its prime factors into * groups of consecutive primes,
 *            and for each group uses the * recurrence * * r(1) = e1 + 1 *
 *            r(2) = e1*e2 + e1 + e2 + 1 * r(3) = e1*e2 + e1*e...
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 9747 (2026-11-01)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Computes the group value for exponents ex[0..len-1] */
static seq_t group_value(const uint64_t *ex, int len)
{
    if (len == 0)
        return (seq_t)1;

    seq_t r1 = (seq_t)ex[0] + 1;
    if (len == 1)
        return r1;

    seq_t r2 = (seq_t)ex[0] * ex[1] + ex[0] + ex[1] + 1;
    if (len == 2)
        return r2;

    seq_t r3 = (seq_t)ex[0] * ex[1] + (seq_t)ex[0] * ex[2] +
               (seq_t)ex[1] * ex[2] + ex[0] + ex[1] + ex[2] + 1;
    if (len == 3)
        return r3;

    seq_t prev2 = r1;   /* r(i-3)   */
    seq_t prev1 = r2;   /* r(i-2)   */
    seq_t cur   = r3;   /* r(i-1)   */

    for (int i = 3; i < len; ++i) {
        seq_t ei   = (seq_t)ex[i];
        seq_t eip1 = (seq_t)ex[i - 1];
        seq_t new_val = cur + ei * prev1 + ei * eip1 * prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = new_val;
    }
    return cur;
}

seq_t a(idx_t n)
{
    /* Handle n = 1 explicitly */
    if (n == 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    /* Enough slots for any factorization of a 64‑bit integer */
    uint64_t primes[20];
    uint64_t exponents[20];
    size_t cnt = 0;

    factor_u64(nn, primes, exponents, &cnt);

    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < cnt; ) {
        idx_t j = i;
        /* Extend the group while the primes are consecutive */
        while (j + 1 < cnt &&
               next_prime(primes[j]) == primes[j + 1])
            ++j;

        int len = (int)(j - i + 1);
        seq_t grp_val = group_value(&exponents[i], len);
        result *= grp_val;

        i = j + 1;
    }

    return result;
}

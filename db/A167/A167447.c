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
 * Range:     u8 n <= 9747, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   factor_u64, group_value, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/group_value.h"

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

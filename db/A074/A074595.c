/*
 * A074595  Number of prime factors of numbers that are not prime powers (with
 *          multiplicity).
 * https://oeis.org/A074595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A024619(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Number of prime factors with multiplicity of the nth non‑prime‑power) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_power.h"

/* Count prime factors of n with multiplicity (Ω(n)). */
static int count_prime_factors(uint64_t n)
{
    int cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    if (n > 1) /* final prime factor */
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    idx_t target = n;
    idx_t count  = 0;
    uint64_t m   = 2;

    while (1) {
        if (!is_prime_power(m)) {
            ++count;
            if (count == target)
                break;
        }
        ++m;
    }

    return (seq_t)count_prime_factors(m);
}

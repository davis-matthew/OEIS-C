/*
 * A193509  Number of odd divisors of Omega(n).
 * https://oeis.org/A193509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors of Ω(n) (big‑omega) where Ω(n) =
 *            total number of prime factors of n with multiplicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)                 /* Ω(1) = 0  ⇒  a(1) = 0 */
        return 0;

    /* Factor n into primes and exponents (big‑omega) */
    uint64_t primes[64];        /* enough for any n ≤ 2^64 – 1 */
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += exps[i];

    if (omega == 0)            /* n = 1 case handled above, but guard anyway */
        return 0;

    /* Remove all factors 2 → keep only the odd part of Ω(n) */
    while ((omega & 1ULL) == 0)
        omega >>= 1;

    /* Count all divisors of the odd part: they are exactly the odd divisors
       of Ω(n).  divisor_count_u64 returns a seq_t value. */
    return divisor_count_u64(omega);
}

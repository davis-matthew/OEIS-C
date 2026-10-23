/*
 * A115069  a(n) = 3^b(n), where b(n) is #{primes p=1 mod 3 dividing n}.
 * https://oeis.org/A115069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 3^#{primes p ≡ 1 (mod 3) dividing n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative: a(p^e)=3 if p ≡ 1(mod3), otherwise 1.
   Therefore a(n) = 3^c where c is the number of distinct prime factors of
   n that are 1 modulo 3. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow3.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        /* The OEIS offset is 1, so values for n < 1 are irrelevant.
           Return 1 (3^0) to stay well‑defined. */
        return pow3((idx_t)0);
    }

    /* a uint64_t number has at most 64 distinct prime factors. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt;

    /* Factor n.  factor_u64 writes into primes, exponents, and cnt. */
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    /* Count primes that are 1 mod 3. */
    unsigned int c = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] % 3U == 1U) {
            ++c;
        }
    }

    /* Return 3^c as a seq_t value. */
    return pow3((idx_t)c);
}

/*
 * A141220  Write the n-th nonprime (A018252(n)) as a product of primes;
 *          increase one copy of the largest prime by 2 and decrease one copy
 *          of the smallest prime by 1, multiply the resulting numbers.
 * https://oeis.org/A141220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the prime factors of the n‑th nonprime, with
 *            one copy of the smallest prime factor decreased by 1 and one
 *            copy of the largest prime factor increased by 2
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Offset safety: a(0) is undefined, but we return 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Find the n‑th nonprime (A018252). */
    idx_t count = 0;
    idx_t candidate = 1;
    while (count < n) {
        if (!is_prime(candidate)) {
            ++count;
            if (count == n) {
                break;
            }
        }
        ++candidate;
    }
    uint64_t m = (uint64_t)candidate;   /* the n‑th nonprime */

    if (m == 1) {                      /* 1 has no prime factors */
        return (seq_t)1;
    }

    /* Factor m into primes and exponents.  The list is sorted ascending
       in most utility implementations. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   cnt;
    factor_u64(m, primes, exps, &cnt);

    /* Compute the adjusted product. */
    seq_t product = (seq_t)1;

    if (cnt == 1) {
        /* Single prime factor p with exponent e. */
        seq_t p = (seq_t)primes[0];
        uint64_t e = exps[0];

        /* One copy decreased by 1 and one copy increased by 2. */
        product *= (p - 1);
        product *= (p + 2);

        /* Remaining copies (if any) are unchanged. */
        for (uint64_t k = 2; k < e; ++k) {
            product *= p;
        }
    } else {
        /* Smallest prime factor (index 0). */
        seq_t p_small = (seq_t)primes[0];
        product *= (p_small - 1);
        for (uint64_t k = 1; k < exps[0]; ++k) {
            product *= p_small;
        }

        /* Middle prime factors. */
        for (size_t i = 1; i + 1 < cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            for (uint64_t k = 0; k < exps[i]; ++k) {
                product *= p;
            }
        }

        /* Largest prime factor (index cnt-1). */
        seq_t p_large = (seq_t)primes[cnt - 1];
        product *= (p_large + 2);
        for (uint64_t k = 1; k < exps[cnt - 1]; ++k) {
            product *= p_large;
        }
    }

    return product;
}

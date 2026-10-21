/*
 * A054506  Log_b 3 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054506
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = discrete logarithm base g of 3 modulo the n-th prime,
 *            where g is the smallest primitive root modulo that prime
 * Range:     u8 n <= 54, u16 n <= 4885, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_unique, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_unique.h"

/* The sequence is defined for n ≥ 3; for n < 3 the function
   returns 0. */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    uint32_t prime = get_prime(n);          /* p = n-th prime   */
    if (prime <= 3)                         /* p = 2 or 3 never occurs for n ≥ 3 */
        return 0;

    /* Find the smallest primitive root modulo prime. */
    uint32_t factors[32];
    size_t nf;
    factor_unique(prime - 1, factors, &nf);

    seq_t g = 2;
    for (;; ++g)
    {
        bool ok = true;
        for (size_t i = 0; i < nf && ok; ++i)
        {
            uint32_t q = factors[i];
            uint64_t exp = ((uint64_t)prime - 1) / q;
            if (mod_pow(g, exp, prime) == 1)
                ok = false;
        }
        if (ok)
            break;
    }

    /* Compute discrete logarithm of 3 base g modulo prime. */
    seq_t cur = 1;
    seq_t k   = 0;
    while (cur != 3)
    {
        cur = (seq_t)(((unsigned long long)cur * g) % prime);
        ++k;
    }

    return k;
}

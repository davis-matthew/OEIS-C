/*
 * A211453  (p-1)/x, where p = prime(n) and x = ord(8,p), the smallest
 *          positive integer such that 8^x == 1 mod p.
 * https://oeis.org/A211453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/ord_8_prime(n) (ord is the multiplicative
 *            order of 8 modulo the prime)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* special treatment for the first prime 2 */
    uint32_t p = get_prime(n);          /* p is the n-th prime (1‑based) */
    if (p == 2) return 0;              /* (2-1)/ord = 0 because no order exists */

    uint64_t m = (uint64_t)p - 1;        /* p-1 is even for odd primes */

    /* factor m */
    uint64_t primes[64], exps[64];
    size_t    cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute multiplicative order of 8 modulo p */
    uint64_t ord = m;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        while (ord % q == 0) {
            uint64_t exp = ord / q;
            if (mod_pow((seq_t)8, exp, (seq_t)p) == 1)
                ord = exp;
            else
                break;
        }
    }

    /* result as seq_t */
    return ((seq_t)(p - 1)) / ((seq_t)ord);
}

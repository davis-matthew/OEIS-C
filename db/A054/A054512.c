/*
 * A054512  Log_b 9 where b is smallest primitive root (A001918) mod n-th
 *          prime.
 * https://oeis.org/A054512
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = discrete logarithm base the smallest primitive root of 9
 *            modulo the n-th prime
 * Range:     u8 n <= 54, u16 n <= 6346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the n-th prime using get_prime, find the smallest primitive root
   of that prime by testing successive integers against the prime factorization
   of (p-1). Then we compute the discrete logarithm by simple repeated
   multiplication modulo p. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* get the n-th prime, n >= 5 */
    uint64_t p = (uint64_t)get_prime(n);

    /* factorize p-1 */
    uint64_t fac[64];
    int exp[64];
    int cnt = factorize(p - 1, fac, exp);

    /* find smallest primitive root */
    uint64_t g;
    for (g = 2; ; ++g)
    {
        bool ok = true;
        for (int i = 0; i < cnt && ok; ++i)
        {
            uint64_t q = fac[i];
            seq_t val = mod_pow((seq_t)g, (p - 1) / q, (seq_t)p);
            if (val == 1)
                ok = false;
        }
        if (ok)
            break;
    }

    /* compute discrete log base g of 9 modulo p */
    seq_t cur = 1;
    seq_t target = (seq_t)9 % (seq_t)p;
    seq_t e = 0;
    while (cur != target)
    {
        cur = (seq_t)((unsigned __int128)cur * g % p);
        ++e;
    }
    return e;
}

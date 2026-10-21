/*
 * A072963  In prime factorization of n replace all single (i.e., non-twin)
 *          primes with 1.
 * https://oeis.org/A072963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e || n} (p^e if p±2 is prime else 1)
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 1;

    uint64_t fac[20];
    int exp[20];
    int sz = 0;
    uint64_t nn = (uint64_t)n;
    factor_uint64(nn, fac, exp, &sz);

    seq_t res = 1;
    for (int i = 0; i < sz; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];
        if (!is_prime(p - 2) && !is_prime(p + 2))
            continue;                  /* p is not a twin prime */

        for (int j = 0; j < e; ++j)
            res *= (seq_t)p;           /* multiply p^e into result */
    }
    return res;
}

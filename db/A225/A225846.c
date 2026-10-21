/*
 * A225846  Denominator of c(n) = 2^(2*n)*(2^(2*n) - 1)/(2*n)!, a coefficient
 *          used in the expansion of tan(x) as Sum_{n>=1}
 *          c(n)*|Bernoulli(2*n)|*x^(2*n-1).
 * https://oeis.org/A225846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of c(n) = 2^(2*n)*(2^(2*n)-1)/(2*n)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* compute exponent of prime p in N! */
static inline idx_t legendre(idx_t N, uint64_t p)
{
    idx_t exp = 0;
    while (N) {
        N /= p;
        exp += N;
    }
    return exp;
}

/* modular exponentiation for 64‑bit arithmetic */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U) {
            result = (unsigned __int128)result * b % mod;
        }
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    /* limit for sieve: primes up to 2n are sufficient */
    idx_t limit = 2 * n;
    if (limit < 3)
        return (seq_t)1;  /* no odd primes to consider */

    /* Sieve of Eratosthenes on the stack (limit ≤ 20000 for n ≤ 10000) */
    bool        is_composite[20001] = {0};
    uint32_t    primes[20001];
    size_t      prime_cnt = 0;
    for (uint32_t i = 2; i <= (uint32_t)limit; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= (uint64_t)limit) {
                for (uint32_t j = i * i; j <= (uint32_t)limit; j += i)
                    is_composite[j] = true;
            }
        }
    }

    seq_t result = (seq_t)1;  /* initial denominator = 1 */

    /* iterate over odd primes only */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p == 2)
            continue;   /* 2 for even part is always cancelled */

        /* exponent of p in (2n)! */
        idx_t exp_fact = legendre(2 * n, p);

        /* exponent of p in 2^(2n)-1 */
        idx_t exp_num = 0;
        uint64_t mod   = p;
        while (true) {
            if (pow_mod_uint64(2U, 2 * n, mod) != 1U)
                break;
            ++exp_num;
            if (mod > UINT64_MAX / p)
                break;   /* would overflow – stop checking higher powers */
            mod *= p;
        }

        /* remaining exponent to keep in the denominator */
        idx_t rem = exp_fact - exp_num;
        if (rem > 0) {
            seq_t p_pow = seq_pow((seq_t)p, rem);
            result = seq_mul(result, p_pow);
        }
    }
    return result;
}

/*
 * A211451  (p-1)/x, where p = prime(n) and x = ord(6,p), the smallest
 *          positive integer such that 6^x == 1 mod p.
 * https://oeis.org/A211451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/ord(6,prime(n))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (p-1)/x, where p = prime(n) and
 * x is the multiplicative order of 6 modulo p */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto      */
#include "../utils/_prime_limit_for_idx.h"/* _prime_limit_for_idx   */
#include "../utils/mod_pow.h"             /* mod_pow                */

static uint64_t *primes = NULL;   /* array of first primes */
static size_t primes_cnt = 0;     /* number of primes stored */

static void ensure_primes(idx_t n)
{
    if (primes_cnt < (size_t)n)
    {
        idx_t limit = _prime_limit_for_idx(n);
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);
        if (primes) free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
    }
}

static uint64_t reduce_order(uint64_t p, uint64_t order)
{
    uint64_t phi = p - 1;
    uint64_t temp = phi;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i)
    {
        uint64_t q = primes[i];
        if (temp % q == 0)
        {
            /* remove all factors of q from temp */
            while (temp % q == 0) temp /= q;

            /* try to divide order by q as many times as possible */
            while (order % q == 0)
            {
                uint64_t cand = order / q;
                if (mod_pow(6, cand, p) == 1)
                    order = cand;
                else
                    break;
            }
        }
    }
    /* if temp > 1, it is a prime factor > sqrt(phi) */
    if (temp > 1)
    {
        uint64_t q = temp;
        while (order % q == 0)
        {
            uint64_t cand = order / q;
            if (mod_pow(6, cand, p) == 1)
                order = cand;
            else
                break;
        }
    }
    return order;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, leave safe */
    ensure_primes(n);

    uint64_t p = primes[n - 1];      /* p = prime(n) */

    /* For p=2 or p=3, 6 is not coprime, define result as 0 */
    if (p == 2 || p == 3) return 0;

    uint64_t ord = reduce_order(p, p - 1);
    seq_t res = (seq_t)(p - 1) / ord;
    return res;
}

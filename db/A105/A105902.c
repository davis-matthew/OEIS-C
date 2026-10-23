/*
 * A105902  Primes for which -30 is a primitive root.
 * https://oeis.org/A105902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   add_uint64, modmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A105902 – primes p such that −30 is a primitive root modulo p
 * The function returns the n‑th such prime (n ≥ 1).  It is memoised
 * so calls may arrive in any order.  The implementation generates
 * primes incrementally and tests the primitive‑root condition for
 * −30 modulo each candidate. */

/* required includes for any build */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/add_uint64.h"
#include "../utils/modmul.h"

/* modular exponentiation */
static uint64_t modpow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1U)
            r = modmul(r, a, m);
        a = modmul(a, a, m);
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for offset 0 */

    /* static state: prime list and good‑prime list */
    static uint64_t *primes    = NULL;
    static idx_t    primes_cnt = 0, primes_cap = 0;
    static uint64_t *good      = NULL;
    static idx_t    good_cnt   = 0, good_cap   = 0;
    static uint64_t candidate  = 3;           /* start from smallest odd prime > 2 */

    /* initialise with 2 */
    if (primes_cnt == 0)
        add_uint64(&primes, &primes_cnt, &primes_cap, 2);

    /* generate sufficient good primes */
    while (good_cnt < n)
    {
        uint64_t p = candidate;
        candidate += 2;                 /* next odd number */

        /* simple trial division to test primality */
        bool is_prime = true;
        for (idx_t i = 0; i < primes_cnt && primes[i] * primes[i] <= p; ++i)
        {
            if (p % primes[i] == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        /* store prime for future factorisations */
        add_uint64(&primes, &primes_cnt, &primes_cap, p);

        /* skip primes dividing 30 */
        if (p <= 5)
            continue;

        /* base = −30 mod p */
        uint64_t base = (p - (30 % p)) % p;
        if (base == 0)
            continue;   /* not coprime to p */

        /* factor p−1 using known primes */
        uint64_t m = p - 1;
        uint64_t tmp = m;
        bool ok = true;

        for (idx_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i)
        {
            uint64_t q = primes[i];
            if (tmp % q == 0)
            {
                /* distinct prime factor q */
                if (modpow(base, m / q, p) == 1)
                {
                    ok = false;
                    break;
                }
                /* divide out all copies of q */
                while (tmp % q == 0)
                    tmp /= q;
            }
        }
        if (ok && tmp > 1)   /* remaining prime factor */
        {
            if (modpow(base, m / tmp, p) == 1)
                ok = false;
        }

        if (ok)
            add_uint64(&good, &good_cnt, &good_cap, p);
    }

    return (seq_t)good[n - 1];
}

/*
 * A094593  a(n) = (p-1)/x, where p = prime(n) and x = ord(3,p), the smallest
 *          positive integer such that 3^x == 1 mod p.
 * https://oeis.org/A094593
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/ord(3,prime(n)) (with offset 3)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_uint64, get_prime, modpow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative order of 3 modulo a prime by trial
 * division of its Euler totient, using the helper factor_uint64
 * and modpow64 functions.  The sequence values are returned in
 * seq_t, the type provided by the build.  For n < 3 the function
 * returns 0. */
#include "../utils/get_prime.h"
#include "../utils/factor_uint64.h"
#include "../utils/modpow64.h"

/* local helper: multiplicative order of 3 modulo a prime */
static uint64_t order_of_3(uint32_t p)
{
    uint64_t phi  = (uint64_t)p - 1ULL;
    uint64_t ord  = phi;
    uint64_t fac[20];
    int      exp[20];
    int      cnt = 0;

    factor_uint64(phi, fac, exp, &cnt);

    for (int i = 0; i < cnt; ++i)
    {
        uint64_t q = fac[i];
        while (ord % q == 0ULL)
        {
            uint64_t cand = ord / q;
            if (modpow64(3ULL, cand, p) == 1ULL)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

seq_t a(idx_t n)
{
    /* the OEIS offset is 3: a(3) is the first listed term */
    if (n < 3ULL) return (seq_t)0;

    /* get the n-th prime (1–indexed) */
    uint32_t p = get_prime(n);

    /* compute ord_3(p) */
    uint64_t ord = order_of_3(p);

    /* compute (p-1)/ord, cast to seq_t */
    seq_t res = ((seq_t)p - (seq_t)1) / (seq_t)ord;
    return res;
}

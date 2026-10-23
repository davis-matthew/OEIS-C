/*
 * A019382  Primes with primitive root 58.
 * https://oeis.org/A019382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p for which 58 is a primitive root modulo p
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Required utilities */
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

static unsigned *prime_factors = NULL;
static size_t      prime_factors_cnt = 0;
static bool        initialized = false;

/* Create a table of small primes that will be used for factorising p-1.  */
static void init_prime_factors(void)
{
    if (initialized) return;
    /* 100 000 is comfortably larger than sqrt(p) for all primes we
       will encounter when generating the first 10 000 terms. */
    idx_t limit = 100000;
    uint64_t *p_list;
    size_t    p_cnt;
    build_primes_upto(limit, &p_list, &p_cnt);

    prime_factors = (unsigned*)malloc(p_cnt * sizeof(unsigned));
    if (!prime_factors) abort();          /* allocation failure */
    for (size_t i = 0; i < p_cnt; ++i)
        prime_factors[i] = (unsigned)p_list[i];
    prime_factors_cnt = p_cnt;

    free(p_list);
    initialized = true;
}

/* true if 58 is a primitive root modulo the prime p.  */
static bool is_primitive_root(uint64_t p)
{
    if (p == 2) return false;            /* 58 mod 2 = 0, not a generator */
    if (58ULL % p == 0) return false;    /* p divides 58, cannot be a primitive root */

    uint64_t phi = p - 1;
    uint64_t tmp = phi;

    /* Test all distinct prime divisors of phi.  */
    for (size_t i = 0; i < prime_factors_cnt && (uint64_t)prime_factors[i] * prime_factors[i] <= tmp; ++i)
    {
        uint64_t q = prime_factors[i];
        if (tmp % q == 0)
        {
            uint64_t exp = phi / q;
            seq_t r = mod_pow((seq_t)58, exp, (seq_t)p);
            if (r == (seq_t)1) return false;

            /* Remove all copies of q from tmp.  */
            while (tmp % q == 0) tmp /= q;
        }
    }

    /* Remaining factor, if any, is prime. */
    if (tmp > 1)
    {
        uint64_t exp = phi / tmp;
        seq_t r = mod_pow((seq_t)58, exp, (seq_t)p);
        if (r == (seq_t)1) return false;
    }

    return true;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                        */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but return 0 to satisfy type */

    init_prime_factors();

    idx_t found = 0;
    uint64_t candidate = 5;               /* the first odd prime that could work */

    for (;;)
    {
        if (is_prime(candidate) && is_primitive_root(candidate))
        {
            ++found;
            if (found == n) return (seq_t)candidate;
        }
        candidate += 2;                   /* skip even numbers */
    }
}

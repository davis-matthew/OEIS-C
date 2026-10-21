/*
 * A213278  Least common multiple of A001175(n) and n.
 * https://oeis.org/A213278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 192, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   fib_mod_pair_u64, pisano_period_prime, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = lcm( pisano_period(n) , n )
 * where pisano_period(n) is the period of the Fibonacci sequence modulo n
 * (OEIS A001175).  The period for a composite n is the lcm of the periods
 * of the prime‑power factors of n, and for a prime power p^e (p≠2,5)
 * the period equals pisano_period(p) * p^(e−1).  The function below
 * computes the period with a small trial‑division factorisation and a
 * fast‑doubling Fibonacci mod algorithm.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/seq_gcd.h"   /* seq_t seq_gcd(seq_t a, seq_t b) */
#include "../utils/fib_mod_pair_u64.h"
#include "../utils/pisano_period_prime.h"

/* ---------- helper: power (uint64 base, unsigned exp) -> seq_t ---------- */
static seq_t pow_uint(uint64_t base, unsigned exp)
{
    seq_t result = 1;
    seq_t b      = (seq_t)base;
    while (exp)
    {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* ---------- prime factorisation (uint64_t) ---------- */
static void factor_uint64(uint64_t n,
                          uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }
    if (n > 1)
    {
        primes[*cnt] = n;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

/* ---------- lcm of two seq_t values ---------- */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = seq_gcd(a, b);
    return (a / g) * b;
}

/* ======================  main sequence function ====================== */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* pisano_period(1)=1, lcm(1,1)=1 */

    /* factor n */
    uint64_t primes[32], exps[32];
    size_t   num_factors;
    factor_uint64((uint64_t)n, primes, exps, &num_factors);

    /* compute period(n) as lcm of prime‑power periods */
    seq_t period_n = 1;
    for (size_t idx = 0; idx < num_factors; ++idx)
    {
        uint64_t p   = primes[idx];
        uint64_t exp = exps[idx];
        seq_t    per;

        if (p == 2)
        {
            per = (seq_t)3 * pow_uint(2, (unsigned)exp - 1);
        }
        else if (p == 5)
        {
            per = (seq_t)20 * pow_uint(5, (unsigned)exp - 1);
        }
        else
        {
            uint64_t t_p = pisano_period_prime(p);
            per = (seq_t)t_p * pow_uint(p, (unsigned)exp - 1);
        }

        period_n = lcm_seq(period_n, per);
    }

    /* finally a(n) = lcm(period(n), n) */
    seq_t result = lcm_seq(period_n, (seq_t)n);
    return result;
}

/*
 * A064143  Sum of unitary prime divisors of binomial(n, floor(n/2)).
 * https://oeis.org/A064143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1032, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of unitary prime divisors of binomial(n, floor(n/2))
 * That is, for each prime p, if the exponent of p in binomial(n,k)
 * (k=floor(n/2)) equals 1, p is added to the sum.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* maximum number of primes needed for n up to about 10 000 */
#define MAX_PRIMES 2000

/* static storage for primes */
static uint32_t primes[MAX_PRIMES];
static size_t   prime_cnt = 0;
static idx_t    sieve_limit = 0;

/* ------------------------------------------------------------------ */
/*   simple incremental sieve of Eratosthenes up to 'limit'          */
/* ------------------------------------------------------------------ */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    for (idx_t num = sieve_limit + 1; num <= limit; ++num) {
        if (num < 2)
            continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > (uint64_t)num)
                break;
            if (num % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt < MAX_PRIMES)
                primes[prime_cnt++] = (uint32_t)num;
        }
    }
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/*   v_p(m!) = sum_{i>=1} floor(m / p^i)                               */
/* ------------------------------------------------------------------ */
static idx_t vp_factorial(idx_t m, uint32_t p)
{
    idx_t res = 0;
    while (m) {
        m /= p;
        res += m;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*   Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1)         /* a(1) = 0 by definition */
        return (seq_t)0;

    idx_t k = n / 2;          /* floor(n/2) */
    ensure_primes(n);         /* primes up to n */

    seq_t sum = 0;            /* result accumulator */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > n)
            break;
        idx_t e = vp_factorial(n, p)
                - vp_factorial(k, p)
                - vp_factorial(n - k, p);
        if (e == 1)
            sum += (seq_t)p;
    }

    return sum;
}

/*
 * A227610  Number of ways 1/n can be expressed as the sum of three distinct
 *          unit fractions: 1/n = 1/x + 1/y + 1/z satisfying 0 < x < y < z.
 * https://oeis.org/A227610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 1/n as 1/x+1/y+1/z with 0 <
 *            x < y < z (no simple closed form is known; algorithm follows
 *            from the identity (p*y - q)*(p*z - q) = q^2 where p = x-n, q =
 *            n*x
 * Range:     u8 n <= 19, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prime sieve up to LIMIT for factorisation */
#define PRIME_SIEVE_LIMIT 200000
static uint32_t primes[PRIME_SIEVE_LIMIT];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool is_comp[PRIME_SIEVE_LIMIT + 1];
    memset(is_comp, 0, sizeof(is_comp));
    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!is_comp[i]) primes[prime_cnt++] = (uint32_t)i;
    primes_ready = true;
}

/* Euclidean gcd for uint64_t */
static inline uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Factorise n using the precomputed prime list.
   The prime factors and their exponents are stored in factor_primes
   and factor_exp arrays. Returns the number of distinct prime factors. */
static size_t factorise(uint64_t n,
                       uint64_t *factor_primes,
                       unsigned *factor_exp)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if ((n % p) == 0) {
            unsigned exp = 0;
            while ((n % p) == 0) {
                n /= p;
                ++exp;
            }
            factor_primes[cnt] = p;
            factor_exp[cnt] = exp;
            ++cnt;
        }
    }
    if (n > 1) {
        factor_primes[cnt] = n;
        factor_exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Recursive enumeration of divisors of q'^2. For each divisor u ≤ q',
   count if it yields a valid triple (x,y,z).  The result is accumulated
   in *cnt_ptr. */
static void rec_divisors(const uint64_t *pf,
                         const unsigned  *exp,
                         size_t          nfacts,
                         size_t          idx,
                         uint64_t        cur,
                         uint64_t        qprime,
                         uint64_t        qprime_sq,
                         uint64_t        pprime,
                         uint64_t        x,
                         uint64_t        *cnt_ptr)
{
    if (idx == nfacts) {
        if (cur > qprime) return;                   /* only divisors up to sqrt(q'^2) needed */
        if ( (cur + qprime) % pprime != 0 ) return; /* u must satisfy congruence */
        uint64_t y = (cur + qprime) / pprime;
        if (y <= x) return;                         /* must be distinct and > x */
        uint64_t v = qprime_sq / cur;
        if (cur == v) return;                      /* gives y == z */
        uint64_t z = (v + qprime) / pprime;
        if (z <= y) return;                        /* ensure y < z */
        ++(*cnt_ptr);
        return;
    }

    uint64_t p = pf[idx];
    unsigned e = exp[idx] * 2;        /* exponent doubled for q'^2 */
    uint64_t val = cur;
    for (unsigned i = 0; i <= e; ++i) {
        rec_divisors(pf, exp, nfacts, idx + 1, val,
                     qprime, qprime_sq, pprime, x, cnt_ptr);
        if (i == e) break;
        val *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_primes();                          /* initialise prime list once  */

    uint64_t total = 0;
    for (idx_t x = n + 1; x <= 3 * n; ++x) {
        idx_t p = x - n;
        idx_t q = n * x;                    /* fits in 64 bits for n≤1e4   */
        idx_t g = ugcd(p, q);
        uint64_t pprime = p / g;
        uint64_t qprime = q / g;
        if (pprime == 0) continue;         /* should not happen    */

        /* factorise qprime */
        uint64_t factor_primes[20];
        unsigned factor_exp[20];
        size_t nfacts = factorise(qprime, factor_primes, factor_exp);

        uint64_t qprime_sq = qprime * qprime;
        uint64_t cnt = 0;
        rec_divisors(factor_primes, factor_exp, nfacts,
                     0, 1, qprime, qprime_sq,
                     pprime, x, &cnt);
        total += cnt;
    }
    return (seq_t)total;
}

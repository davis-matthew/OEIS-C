/*
 * A099642  a(n) is the smallest nonprime k such that tau(k + n) = tau(k) + n
 *          , where tau(n) is the number of divisors of n (A000005).
 * https://oeis.org/A099642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A099642:
 *  a(n) is the smallest non‑prime k such that τ(k + n) = τ(k) + n,
 *  where τ(m) is the number of divisors of m. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- global cache for primes -------------------------------------------*/
static bool *prime_sieve = NULL;      /* true  → number is prime      */
static idx_t  sieve_limit = 0;        /* current size of prime_sieve  */
static uint64_t *prime_list = NULL;   /* array of primes up to limit   */
static size_t   prime_count = 0;      /* number of primes stored      */

/*--- extend the prime sieve to at least 'limit' (inclusive) ------------*/
static void
ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* allocate new sieve array */
    bool *new_sieve = (bool *)malloc((limit + 1U) * sizeof(bool));
    if (!new_sieve) abort();

    for (idx_t i = 0; i <= limit; ++i)
        new_sieve[i] = true;
    if (limit >= 0) new_sieve[0] = false;
    if (limit >= 1) new_sieve[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (new_sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = false;

    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = limit;

    /* build list of primes up to new limit */
    size_t est = limit / 10 + 100;          /* rough over–estimate   */
    uint64_t *new_list = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!new_list) abort();
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_sieve[i]) {
            if (cnt == est) {               /* grow array if needed */
                est <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(new_list, est * sizeof(uint64_t));
                if (!tmp) abort();
                new_list = tmp;
            }
            new_list[cnt++] = (uint64_t)i;
        }
    free(prime_list);
    prime_list = new_list;
    prime_count = cnt;
}

/*--- integer square root (floor) for idx_t --------------------------------*/
static idx_t
int_sqrt(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = mid + 1; continue; }
        if (mid > x / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

/*--- primality test using the maintained prime list ---------------------*/
static bool
is_prime_uint64(uint64_t v)
{
    if (v <= 1) return false;
    if (v <= sieve_limit) return prime_sieve[v];
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > v) break;
        if (v % p == 0) return false;
    }
    return true;
}

/*--- divisor count τ(n) by prime factorisation ---------------------------*/
static seq_t
tau_of_uint64(uint64_t n)
{
    if (n <= 1u) return (seq_t)1;
    seq_t result = 1;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            unsigned int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= (seq_t)(exp + 1);
        }
    }
    if (temp > 1)   /* remaining prime factor */
        result *= 2;
    return result;
}

/*--- main sequence function ---------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n≥1 */

    for (idx_t k = 1; ; ++k) {
        /* ensure primes up to sqrt(k + n) for factorisation */
        idx_t lim = int_sqrt(k + n);
        if (lim < 1) lim = 1;
        ensure_primes(lim + 1);           /* +1 to be safe */

        /* skip primes; 1 is treated as non‑prime */
        if (k >= 2 && is_prime_uint64((uint64_t)k))
            continue;

        /* compute τ(k) and τ(k+n) */
        seq_t tau_k  = tau_of_uint64((uint64_t)k);
        seq_t tau_kn = tau_of_uint64((uint64_t)(k + n));

        if (tau_kn == tau_k + (seq_t)n)
            return (seq_t)k;
    }
}

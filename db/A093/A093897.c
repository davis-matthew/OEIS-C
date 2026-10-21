/*
 * A093897  a(n) = A093896(n)! / n^n.
 * https://oeis.org/A093897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A093896(n)! / n^n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093896(n) is the least m such that m! is divisible by n^n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* global state for primes */
static idx_t *g_primes     = NULL;
static size_t g_prime_cnt  = 0;
static const idx_t PRIME_LIMIT = 400000;   /* sufficient for n ≤ 10000 */

/* initialise prime table once */
static void init_primes(void)
{
    if (g_primes != NULL)
        return;

    /* sieve */
    uint8_t *sieve = (uint8_t*)malloc(PRIME_LIMIT + 1);
    for (idx_t i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;

    /* collect primes */
    g_primes     = (idx_t*)malloc(PRIME_LIMIT * sizeof(idx_t));
    g_prime_cnt  = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            g_primes[g_prime_cnt++] = i;

    free(sieve);
}

/* exponent of p in m! (Legendre's formula) */
static idx_t exp_in_fact(idx_t m, idx_t p)
{
    idx_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* minimal m with exp_in_fact(m,p) ≥ needed */
static idx_t min_m_for_prime(idx_t p, idx_t needed)
{
    idx_t low  = 1;
    idx_t high = needed * p;        /* upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (exp_in_fact(mid, p) >= needed)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* fast exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* compute m! using prime factorisation */
static seq_t factorial_of_m(idx_t m)
{
    seq_t fact = (seq_t)1;
    for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= m; ++i) {
        idx_t p = g_primes[i];
        idx_t exp = 0;
        idx_t power = p;
        while (power <= m) {
            exp += m / power;
            if (power > m / p) break;     /* prevent overflow */
            power *= p;
        }
        if (exp > 0) {
            seq_t pp = pow_seq_t((seq_t)p, exp);
            fact *= pp;
        }
    }
    return fact;
}

/* cache of computed terms */
static seq_t *cache      = NULL;
static bool *computed    = NULL;
static size_t cache_size = 0;

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused but safe */

    init_primes();

    /* extend cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache      = (seq_t*)realloc(cache,      new_size * sizeof(seq_t));
        computed   = (bool*)realloc(computed,   new_size * sizeof(bool));
        for (size_t i = cache_size; i < new_size; ++i)
            computed[i] = false;
        cache_size = new_size;
    }
    if (computed[n])
        return cache[n];

    /* factor n */
    idx_t tmp = n;
    idx_t m_needed = 0;

    for (size_t i = 0; i < g_prime_cnt && g_primes[i] * g_primes[i] <= tmp; ++i) {
        idx_t p = g_primes[i];
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            idx_t needed = e * n;
            idx_t mp = min_m_for_prime(p, needed);
            if (mp > m_needed)
                m_needed = mp;
        }
    }
    if (tmp > 1) {                       /* remaining prime factor */
        idx_t p = tmp;
        idx_t e = 1;
        idx_t needed = e * n;
        idx_t mp = min_m_for_prime(p, needed);
        if (mp > m_needed)
            m_needed = mp;
    }

    /* compute m! */
    seq_t fact = factorial_of_m(m_needed);

    /* compute n^n */
    seq_t n_pow = pow_seq_t((seq_t)n, n);

    seq_t result = fact / n_pow;

    cache[n] = result;
    computed[n] = true;
    return result;
}

/*
 * A064778  Largest m such that 1..m all divide n!.
 * https://oeis.org/A064778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest m such that for all k ≤ m, k | n! (OEIS A064778)
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* prime tables (static, shared across calls) */
static uint32_t *prime_list  = NULL; /* array of primes */
static size_t    prime_cnt   = 0;     /* number of primes */
static idx_t     prime_limit = 0;     /* maximum prime value stored */

/* ensure primes up to at least 'limit' are available */
static void
ensure_primes(idx_t limit)
{
    if (prime_limit >= limit) return;

    /* free previous list */
    free(prime_list);

    /* simple sieve */
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_prime = (uint8_t*)malloc(sz);
    if (!is_prime) abort();
    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* count primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_cnt;

    /* allocate storage */
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    free(is_prime);
    prime_limit = limit;
}

/* exponent of prime p in n! (n, p are uint64_t) */
static uint64_t
exp_in_fact(uint64_t n, uint64_t p)
{
    uint64_t exp = 0;
    uint64_t d   = p;
    while (d <= n) {
        exp += n / d;
        if (d > n / p) break;   /* avoid overflow */
        d *= p;
    }
    return exp;
}

/* static array to hold exponents for primes up to current limit */
/* value 0 means prime > current n */
static uint64_t *exp_fact = NULL;
static size_t    exp_cnt  = 0;

/* utility: find first prime > n (returns prime or 0 if none) */
static uint32_t
next_prime_gt(idx_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] > n) return prime_list[i];
    }
    return 0;
}

/* check whether m satisfies the condition */
static bool
valid_m(idx_t m, idx_t n)
{
    /* iterate over primes <= m */
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        uint64_t e = 0;
        if (p <= (uint32_t)n) {
            /* exponent in n! */
            e = exp_fact[i] + 1;  /* need e_n(p)+1 */
        } else {
            /* prime > n : exponent 0, thus need p > m. */
            if ((idx_t)p <= m) return false;   /* violation */
            continue;                          /* no further restriction */
        }

        /* compute p^e, stop if exceeds m */
        uint64_t val = 1;
        for (uint64_t j = 0; j < e; ++j) {
            /* overflow guard: if val > m / p, then val*p > m */
            if (val > (uint64_t)m / p) {
                val = (uint64_t)m + 1;  /* force exceeding */
                break;
            }
            val *= p;
        }
        if (val <= (uint64_t)m) return false; /* fails */
    }
    return true;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n=0, not used */

    /* upper bound for prime generation: next prime after n plus a safety margin */
    idx_t limit = n + 1000;              /* enough for n ≤ 10000 (prime gaps small) */
    ensure_primes(limit);

    /* allocate or resize exp_fact array */
    if (exp_cnt < prime_cnt) {
        free(exp_fact);
        exp_fact = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
        if (!exp_fact) abort();
        exp_cnt = prime_cnt;
    }

    /* compute exponents for primes ≤ n, zero otherwise */
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] <= (uint32_t)n)
            exp_fact[i] = exp_in_fact((uint64_t)n, (uint64_t)prime_list[i]);
        else
            exp_fact[i] = 0;
    }

    /* upper bound for m: next prime after n minus 1 (or n if none) */
    uint32_t np = next_prime_gt(n);
    idx_t ub = (np ? np - 1 : n);
    if (ub < n) ub = n;                 /* safety */

    idx_t lo = n, hi = ub;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (valid_m(mid, n))
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

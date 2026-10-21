/*
 * A209315  Number of ways to write 2n-1 = p+q with q practical, p and q-p
 *          both prime.
 * https://oeis.org/A209315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209315
   a(n) = number of ways to write 2n-1 = p+q with
          q practical and both p and q-p prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Prime sieve and caching --------------------------------------------- */
static size_t sieve_limit = 0;
static uint8_t *is_prime_array = NULL;   /* 0=non‑prime, 1=prime, length = sieve_limit+1 */
static idx_t *prime_list = NULL;         /* list of primes up to sieve_limit */
static size_t prime_count = 0;

/* ensure that primes up to at least "limit" are available */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* reallocate and recompute sieve up to new limit */
    size_t new_limit = limit;
    uint8_t *new_is_prime = NULL;
    idx_t *new_primes = NULL;

    new_is_prime = (uint8_t *)malloc(new_limit + 1);
    if (!new_is_prime)
        abort();          /* memory error */
    for (size_t i = 0; i <= new_limit; ++i)
        new_is_prime[i] = 1;
    new_is_prime[0] = new_is_prime[1] = 0;

    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (new_is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                new_is_prime[j] = 0;
        }
    }

    size_t new_cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_is_prime[i])
            new_cnt++;

    new_primes = (idx_t *)malloc(new_cnt * sizeof(idx_t));
    if (!new_primes)
        abort();          /* memory error */
    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (new_is_prime[i])
            new_primes[idx++] = (idx_t)i;

    /* free old storage */
    free(is_prime_array);
    free(prime_list);

    /* commit new values */
    is_prime_array = new_is_prime;
    prime_list = new_primes;
    prime_count = new_cnt;
    sieve_limit = new_limit;
}

/* test if a number <= sieve_limit is prime */
static inline bool is_prime_small(idx_t x)
{
    if (x > sieve_limit)
        return false;   /* should not happen */
    return is_prime_array[x] != 0;
}

/* ---- Practical number caching --------------------------------------------- */
static uint8_t *practical_flag = NULL;   /* 0=false, 1=true, 0xFF=unknown */
static size_t practical_cap = 0;

/* ensure practical_flag array can hold numbers up to "limit" */
static void ensure_practical_cache(idx_t limit)
{
    if (limit <= practical_cap)
        return;

    uint8_t *new_flag = (uint8_t *)realloc(practical_flag, (limit + 1) * sizeof(uint8_t));
    if (!new_flag)
        abort();
    /* initialise new entries as unknown */
    for (size_t i = practical_cap; i <= limit; ++i)
        new_flag[i] = (uint8_t)0xFF;
    practical_flag = new_flag;
    practical_cap = limit;
}

/* ---- Practical number test ---------------------------------------------- */
static bool is_practical(uint64_t n)
{
    if (n == 1)
        return true;

    uint64_t remaining = n;
    uint64_t sigma_prev = 1;          /* product of sigma of processed prime powers */
    bool first = true;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= remaining; ++i) {
        uint64_t p = prime_list[i];
        if (remaining % p != 0)
            continue;

        /* exponent of p */
        uint64_t exp = 0;
        do {
            remaining /= p;
            ++exp;
        } while (remaining % p == 0);

        /* first prime must be 2 */
        if (first) {
            if (p != 2)
                return false;
            first = false;
        } else {
            if (p > sigma_prev + 1)
                return false;
        }

        /* sigma(p^exp) = 1 + p + ... + p^exp */
        uint64_t sigma_part = 1;
        uint64_t pow = 1;
        for (uint64_t j = 0; j < exp; ++j) {
            pow *= p;
            sigma_part += pow;
        }
        sigma_prev *= sigma_part;
    }

    if (remaining > 1) {  /* remaining is a prime factor > sqrt(original n) */
        uint64_t p = remaining;
        if (first) {
            if (p != 2)
                return false;
        } else {
            if (p > sigma_prev + 1)
                return false;
        }
        uint64_t sigma_part = 1 + remaining;
        sigma_prev *= sigma_part;
    }

    return true;
}

/* ---- Sequence function --------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)            /* offset is 1, but accept 0 safely */
        return 0;

    /* M = 2n-1 */
    seq_t Mseq = ((seq_t)n) * ((seq_t)2) - (seq_t)1;
    idx_t M = (idx_t)Mseq;

    /* ensure needed resources up to M */
    ensure_primes(M);
    ensure_practical_cache(M);

    seq_t count = 0;

    /* iterate over primes p <= M-2 */
    for (size_t i = 0; i < prime_count && prime_list[i] <= M - 2; ++i) {
        idx_t p = prime_list[i];
        idx_t d = M - (idx_t)2 * p;           /* d = q - p */
        if (d < 2)
            continue;
        if (!is_prime_small(d))
            continue;

        idx_t q = M - p;                      /* q = 2n-1 - p */

        if (practical_flag[q] == (uint8_t)0xFF) {
            practical_flag[q] = is_practical((uint64_t)q) ? (uint8_t)1 : (uint8_t)0;
        }
        if (practical_flag[q])
            ++count;
    }

    return count;
}

/*
 * A090863  Numbers n such that F(n+1)*F(n-1)*B(2n) is an integer, where
 *          F(k)=k-th Fibonacci number and B(2k)=2k-th Bernoulli number.
 * https://oeis.org/A090863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A090863:
 *  a(n) = the nth positive integer n such that
 *  F(n+1)*F(n-1)*B(2n) is an integer,
 *  where F(k) is the k‑th Fibonacci number and
 *  B(2k) the 2k‑th Bernoulli number.
 *  (The condition is equivalent to: for every prime p with
 *   p‑1 | 2n, either F(n−1) ≡ 0 (mod p) or F(n+1) ≡ 0 (mod p).)
 *  This implementation generates the sequence on demand
 *  and caches results for arbitrary access order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* unsigned integers supplied by the build environment   */
#ifndef USE_BIGINT   /* If this macro is defined the build uses an arbitrary‑precision class */
#  define USE_BIGINT
#endif

/* ----------  Sieve of Eratosthenes  ---------- */
#define SIEVE_LIMIT 1000000

static bool *is_prime_sieve = NULL;   /* primality table          */
static idx_t *prime_list = NULL;      /* list of primes           */
static size_t prime_cnt = 0;          /* number of primes in list */

/* Initialise the sieve once. */
static void init_sieve(void)
{
    if (is_prime_sieve != NULL)
        return;                     /* already initialised */

    /* Allocate memory for the boolean sieve. */
#ifdef __cplusplus
    is_prime_sieve = static_cast<bool*>(calloc(SIEVE_LIMIT + 1, sizeof(bool)));
#else
    is_prime_sieve = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
#endif
    if (is_prime_sieve == NULL)
        exit(EXIT_FAILURE);

    /* Mark all as prime initially. */
#ifdef __cplusplus
    std::fill(is_prime_sieve, is_prime_sieve + SIEVE_LIMIT + 1, true);
#else
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime_sieve[i] = true;
#endif
    is_prime_sieve[0] = is_prime_sieve[1] = false;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime_sieve[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_sieve[j] = false;
        }
    }

    /* Count primes. */
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime_sieve[i])
            ++prime_cnt;

    /* Allocate and store prime list. */
#ifdef __cplusplus
    prime_list = static_cast<idx_t*>(malloc(prime_cnt * sizeof(idx_t)));
#else
    prime_list = (idx_t*)malloc(prime_cnt * sizeof(idx_t));
#endif
    if (prime_list == NULL)
        exit(EXIT_FAILURE);

    size_t pos = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime_sieve[i])
            prime_list[pos++] = i;
}

/* ----------  Fibonacci modulo 64‑bit (fast doubling)  ---------- */
static seq_t fib_mod_u64(uint64_t mod, idx_t n)
{
    if (mod == 1)
        return (seq_t)0;

    uint64_t a = 0;   /* F(0) */
    uint64_t b = 1;   /* F(1) */

    for (int i = 63; i >= 0; --i) {
        /* Compute F(2k) and F(2k+1) from F(k)=a, F(k+1)=b. */
        uint64_t t = (2*b % mod + mod - a) % mod;      /* 2*b - a  */
        uint64_t c = (a * t) % mod;                    /* F(2k)    */
        uint64_t d = (a*a % mod + b*b % mod) % mod;    /* F(2k+1)  */

        if ((n >> i) & 1) {   /* use odd case */
            a = d;
            b = (c + d) % mod;
        } else {               /* use even case */
            a = c;
            b = d;
        }
    }
    return (seq_t)a;   /* F(n) */
}

/* ----------  Property test for a single n  ---------- */
static bool satisfies_property(idx_t n)
{
    idx_t m = n << 1;                 /* 2*n */

    /* Enumerate all divisors of m. */
    idx_t divs[256];                  /* sufficient for m <= 2*10^5 in our use case */
    size_t divc = 0;
    divs[divc++] = 1;

    idx_t temp = m;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        idx_t p = prime_list[i];
        if (temp % p != 0)
            continue;
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        size_t old = divc;
        idx_t mult = 1;
        for (int e = 1; e <= exp; ++e) {
            mult *= p;
            for (size_t j = 0; j < old; ++j)
                divs[divc++] = divs[j] * mult;
        }
    }
    if (temp > 1) {                        /* remaining prime factor */
        idx_t p = temp;
        size_t old = divc;
        for (size_t j = 0; j < old; ++j)
            divs[divc++] = divs[j] * p;
    }

    /* For each divisor d, check if d+1 is prime and test Fibonacci divisibility. */
    for (size_t i = 0; i < divc; ++i) {
        idx_t d = divs[i];
        idx_t p = d + 1;                  /* candidate prime */
        if (p > (idx_t)SIEVE_LIMIT || !is_prime_sieve[p])
            continue;                    /* skip non‑prime */

        seq_t f1 = fib_mod_u64((uint64_t)p, n + 1);
        seq_t f2 = fib_mod_u64((uint64_t)p, n - 1);
        if (f1 == 0 || f2 == 0)
            continue;                    /* condition satisfied for this prime */
        else
            return false;                /* failure for this prime */
    }
    return true;                         /* all required primes satisfied */
}

/* ----------  Cached sequence generator  ---------- */
static seq_t *cache = NULL;         /* cache of computed terms */
static size_t cache_cap = 0;        /* capacity of cache */
static size_t cache_len = 0;        /* number of terms stored */
static idx_t next_trial = 1;        /* next n to test */

/* Expand cache if needed. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *new_cache = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (new_cache == NULL)
        exit(EXIT_FAILURE);
    cache = new_cache;

    /* Initialize new elements to zero / default construct. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;        /* works for both integer and class types */

    cache_cap = new_cap;
}

/* ----------  Main function to the sequence  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* outside the offset, value irrelevant */

    init_sieve();                 /* ensure primes are available */

    /* Grow cache to hold at least n elements. */
    ensure_cache(n);

    /* Generate terms until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (satisfies_property(next_trial))
            cache[cache_len++] = (seq_t)next_trial;
        ++next_trial;
    }

    return cache[n - 1];
}

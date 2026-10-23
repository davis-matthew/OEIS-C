/*
 * A104193  a(n) = prime(n) - Carmichael lambda(n).
 * https://oeis.org/A104193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n) - A002322(n)
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ---------- type definitions supplied by the build ----------
   seq_t  : unsigned integer type (8/16/32/64/128 bits or big int);
   idx_t  : unsigned integer type, uint64_t                         */
/* ---------------------------------------------------------------- */

/* ----------------------------------------------------------------
   Helper : GCD for 64‑bit unsigned integers
   ---------------------------------------------------------------- */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ----------------------------------------------------------------
   Helper : LCM for 64‑bit unsigned integers (using 128‑bit to avoid overflow)
   ---------------------------------------------------------------- */
static inline uint64_t lcm64(uint64_t a, uint64_t b)
{
    if (a == 0 || b == 0) return 0;
    uint64_t g = gcd64(a, b);
    return (uint64_t)((__uint128_t)(a / g) * b);
}

/* ----------------------------------------------------------------
   Prime generation – dynamic sieve
   ---------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count  = 0;      /* number of primes stored */
static size_t   prime_cap    = 0;      /* allocated capacity */
static uint64_t sieve_limit  = 0;      /* highest sieved value */

/* Extend the sieve up to at least 'limit' (inclusive) */
static void build_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;          /* already sufficient */

    /* allocate boolean array for the sieve */
    unsigned char *is_prime = (unsigned char *)calloc(limit + 1, 1);
    if (!is_prime) abort();

    for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    uint64_t sq = (uint64_t)sqrt((double)limit);

    for (uint64_t p = 2; p <= sq; ++p) {
        if (!is_prime[p]) continue;
        for (uint64_t k = p * p; k <= limit; k += p)
            is_prime[k] = 0;
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* enlarge array if needed */
    if (cnt > prime_cap) {
        size_t new_cap = cnt;
        uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!new_list) abort();
        prime_list   = new_list;
        prime_cap    = new_cap;
    }

    /* store primes */
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    prime_count = idx;
    sieve_limit = limit;
    free(is_prime);
}

/* Estimate upper bound for the n‑th prime (Rosser–Schoenfeld) */
static uint64_t estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;                      /* trivial */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (uint64_t)(bound) + 1;
}

/* Ensure that the first 'n' primes are generated */
static void ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_count) return;
    uint64_t limit = estimate_prime_upper_bound(n);
    build_sieve(limit);
    if ((size_t)n > prime_count) {
        /* if estimate was too low, double the range and rebuild */
        while ((size_t)n > prime_count) {
            limit *= 2;
            build_sieve(limit);
        }
    }
}

/* ----------------------------------------------------------------
   Carmichael λ function for 1 ≤ n < 2^64
   ---------------------------------------------------------------- */
static seq_t carmichael_lambda(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t m   = (uint64_t)n;
    uint64_t lam = 1;

    /* Factor m using the prime list (enough primes for sqrt(m) are present) */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p != 0) continue;

        uint64_t cnt = 0;
        uint64_t p_pow = 1;
        while (m % p == 0) {
            m /= p;
            ++cnt;
            if (cnt > 1) p_pow *= p;           /* p^cnt */
        }

        uint64_t li;
        if (p == 2) {
            if (cnt == 1)       li = 1;
            else if (cnt == 2)  li = 2;
            else                li = 1ULL << (cnt - 2); /* 2^(cnt‑2) */
        } else {
            li = p_pow * (p - 1);                /* p^(cnt‑1)·(p‑1) */
        }

        lam = lcm64(lam, li);
    }

    if (m > 1) {                    /* remaining prime factor */
        uint64_t p = m;
        uint64_t li = (p == 2) ? 1 : (p - 1);
        lam = lcm64(lam, li);
    }

    return (seq_t)lam;
}

/* ----------------------------------------------------------------
   Main sequence function
   ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* outside the offset – harmless */

    /* Ensure we have enough primes for prime(n) */
    ensure_primes(n);

    seq_t lam = carmichael_lambda(n);

    seq_t prime_n = (seq_t)prime_list[n - 1];   /* prime(1) = 2 */

    return prime_n - lam;               /* guaranteed non‑negative */
}

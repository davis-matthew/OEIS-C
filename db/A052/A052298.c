/*
 * A052298  Maximal number of distinct prime factors of any composite number
 *          between n-th and (n+1)-st primes.
 * https://oeis.org/A052298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052298:  maximal number of distinct prime factors of any composite
   number between the n‑th and (n+1)-st primes.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------- configuration ------------------------------------------- */
static const size_t SIEVE_LIMIT = 200000;          /* > 10 000‑th prime (104 729) */

/* ----------- global state -------------------------------------------- */
static uint8_t *prime_flag = NULL;     /* 1 = prime, 0 = composite, size SIEVE_LIMIT+1 */
static uint64_t *prime_list = NULL;    /* list of primes up to SIEVE_LIMIT */
static size_t    prime_cnt  = 0;        /* number of primes stored */

/* ----------- initialisation ------------------------------------------ */
static void init_primes(void)
{
    if (prime_flag != NULL)   /* already initialised */
        return;

    /* allocate and mark all as prime */
    prime_flag = (uint8_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (prime_flag == NULL) exit(1);  /* allocation failure */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        prime_flag[i] = 1;

    /* sieve */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }

    /* count primes and build list */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_flag[i]) ++prime_cnt;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (prime_list == NULL) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_flag[i])
            prime_list[idx++] = (uint64_t)i;
}

/* ----------- helpers ---------------------------------------------- */
static bool is_prime(uint64_t x)
{
    if (x > SIEVE_LIMIT) return false;
    return (x >= 2) && prime_flag[x];
}

/* count distinct prime factors of n (n <= SIEVE_LIMIT) */
static seq_t distinct_prime_factors(uint64_t n)
{
    seq_t cnt = 0;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            ++cnt;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) ++cnt;      /* remaining prime factor */
    return cnt;
}

/* ----------- required function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* undefined for n = 0, return 0 */

    init_primes();

    /* ensure we have at least n+1 primes in our list */
    if (prime_cnt <= (size_t)n) {
        /* the chosen SIEVE_LIMIT covers n up to 10000; this should never happen. */
        return (seq_t)0;
    }

    uint64_t p1 = prime_list[n - 1];   /* prime(n) */
    uint64_t p2 = prime_list[n];       /* prime(n+1) */
    seq_t maxc = 0;

    for (uint64_t m = p1 + 1; m < p2; ++m) {
        if (is_prime(m))                /* skip primes */
            continue;
        seq_t c = distinct_prime_factors(m);
        if (c > maxc) maxc = c;
    }
    return maxc;
}

/*
 * A237768  Number of primes p < n with pi(n-p) a Sophie Germain prime, where
 *          pi(.) is given by A000720.
 * https://oeis.org/A237768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{prime p < n | pi(n-p) is a Sophie Germain prime} * A
 *            Sophie Germain prime is a prime m such that 2*m+1 is also prime.
 *            * * The function builds a prime sieve up to its current demand
 *            and * uses a prefix count array to evaluate pi(n-p) in O(1). *
 *            All work is done with the types seq_t and id...
 * Range:     u8 n <= 1261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9219 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Helper from the utils directory that tells whether a seq_t number is prime.
   The header name matches the one in the repository. */

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------------------------------  static cache  ---------------------------------- */
static uint8_t *is_prime_flag = NULL; /* 1 = prime, 0 = composite */
static idx_t  *pi_prefix     = NULL;  /* cumulative prime counts */
static uint64_t *prime_list  = NULL;  /* list of primes */
static size_t   primes_cnt   = 0;
static idx_t    sieve_limit  = 0;

/* Allocate the sieve arrays (C++ needs explicit casts) */
static void build_sieve(idx_t limit)
{
    /* free previous data */
    free(is_prime_flag);
    free(pi_prefix);
    free(prime_list);

    /* allocate the flags array */
    is_prime_flag = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!is_prime_flag) exit(EXIT_FAILURE);

    /* initialise all as prime */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime_flag[i] = 1;
    if (limit >= 0) is_prime_flag[0] = 0;
    if (limit >= 1) is_prime_flag[1] = 0;

    /* simple eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_flag[j] = 0;
        }
    }

    /* prefix counts of primes */
    pi_prefix = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!pi_prefix) exit(EXIT_FAILURE);
    idx_t cumulative = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (i >= 2 && is_prime_flag[i]) ++cumulative;
        pi_prefix[i] = cumulative;
    }

    /* collect primes into a list */
    /* first count them to allocate exactly enough space */
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_flag[i]) ++primes_cnt;

    prime_list = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_flag[i]) prime_list[idx++] = (uint64_t)i;

    sieve_limit = limit;
}

/* Ensure sieve covers at least 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit > sieve_limit)
        build_sieve(limit);
}

/* ----------------------------------  main function  ---------------------------------- */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1)=0. For n < 2 no primes exist. */
    if (n <= 2) return 0;

    /* We need primes p < n and pi(n-p) for n-p up to n-2.
       So a sieve up to n-1 is sufficient. */
    idx_t limit = n - 1;
    ensure_sieve(limit);

    seq_t count = 0;

    /* iterate over all primes less than n */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = (idx_t)prime_list[i];
        if (p >= n) break;          /* just in case */

        idx_t m = pi_prefix[n - p];   /* pi(n-p) */
        seq_t sm = (seq_t)m;          /* promote to seq_t */
        /* Check that sm is a Sophie Germain prime */
        if (is_prime_big(sm) && is_prime_big(sm * 2 + 1))
            count += 1;
    }

    return count;
}

/*
 * A164930  Sum of odd prime divisors of numbers with all odd prime divisors
 *          of the form 6k+5.
 * https://oeis.org/A164930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search / sieve: generates numbers whose odd prime factors are the
   primes ≡5 (mod 6), skips powers of 2 (which have no odd prime factor),
   and returns the sum of the distinct odd prime divisors of the nth such
   number (OEIS A164930). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Cache for the a(n) sequence. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
static size_t a_cache_cap = 0;

/* Allowed primes: 2 and all primes ≡5 (mod 6). */
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Array of generated numbers of the form 2^a * ∏ p_i^{e_i} (allowing
   the prime factor 2).  The first element is 1. */
static uint64_t *numbers = NULL;
static size_t nums_size = 0;
static size_t nums_cap  = 0;

/* For each prime, the index into numbers that produces the next
   candidate multiplication. */
static size_t *idx_array = NULL;

/* -------------------------------------------------------------------------- */
/*  Prime generation (2 and primes ≡5 mod 6, up to a fixed limit).             */
/* -------------------------------------------------------------------------- */
static void init_primes(void) {
    if (prime_count) return;              /* already initialized */

    const uint64_t LIMIT = 200000;        /* sufficient for n ≈ 10 000 */

    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) abort();

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) sieve[j] = true;
        }
    }

    /* Count primes ≡5 (mod 6). */
    size_t count5 = 0;
    for (uint64_t i = 5; i <= LIMIT; i += 6) {
        if (!sieve[i]) ++count5;
    }

    prime_count = 1 + count5;               /* 1 for the prime 2 */
    primes = (uint64_t *)calloc(prime_count, sizeof(uint64_t));
    if (!primes) abort();

    primes[0] = 2;                          /* include the prime 2 */
    size_t pos = 1;
    for (uint64_t i = 5; i <= LIMIT; i += 6) {
        if (!sieve[i]) primes[pos++] = i;
    }

    free(sieve);
}

/* -------------------------------------------------------------------------- */
/*  Dynamic array helpers.                                                   */
/* -------------------------------------------------------------------------- */
static void grow_cache(size_t min_cap) {
    if (a_cache_cap >= min_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 256;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
    a_cache_cap = new_cap;
}

static void grow_numbers(size_t min_cap) {
    if (nums_cap >= min_cap) return;
    size_t new_cap = nums_cap ? nums_cap * 2 : 512;
    if (new_cap < min_cap) new_cap = min_cap;
    uint64_t *tmp = (uint64_t *)realloc(numbers, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    numbers = tmp;
    nums_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/*  Generate the sequence up to a given index.                                */
/* -------------------------------------------------------------------------- */
static void generate_upto(size_t target) {
    if (a_cache_size >= target) return;

    init_primes();

    if (!idx_array) {
        idx_array = (size_t *)calloc(prime_count, sizeof(size_t));
        if (!idx_array) abort();
    }

    /* Initialize numbers array with the first element 1. */
    if (!numbers) {
        grow_numbers(1);
        numbers[0] = 1;
        nums_size = 1;
    }

    while (a_cache_size < target) {
        /* Find the smallest product prime[p] * numbers[idx_array[p]]. */
        __uint128_t min_prod = ~((__uint128_t)0); /* maximum 128‑bit value */
        for (size_t p = 0; p < prime_count; ++p) {
            __uint128_t prod = (__uint128_t)primes[p] *
                               (__uint128_t)numbers[idx_array[p]];
            if (prod < min_prod) min_prod = prod;
        }

        if (min_prod > ( __uint128_t )UINT64_MAX) abort(); /* overflow guard */

        uint64_t cand = (uint64_t)min_prod;

        /* Append candidate to the numbers list. */
        grow_numbers(nums_size + 1);
        numbers[nums_size++] = cand;

        /* Advance indices for primes that produced this candidate. */
        for (size_t p = 0; p < prime_count; ++p) {
            __uint128_t prod = (__uint128_t)primes[p] *
                               (__uint128_t)numbers[idx_array[p]];
            if (prod == min_prod) ++idx_array[p];
        }

        /* Skip pure powers of two (no odd prime factor). */
        if (cand > 1 && (cand & (cand - 1)) != 0) {
            uint64_t pr[64], ex[64];
            size_t cnt;
            factor_u64(cand, pr, ex, &cnt);

            seq_t sum = 0;
            for (size_t i = 0; i < cnt; ++i) {
                if (pr[i] != 2) sum += (seq_t)pr[i];
            }

            grow_cache(a_cache_size + 1);
            a_cache[a_cache_size++] = sum;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*  Public sequence function.                                                 */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n=0, safe guard */
    size_t target = (size_t)n;
    if (a_cache_size < target) generate_upto(target);
    return a_cache[n - 1];
}

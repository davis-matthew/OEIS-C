/*
 * A236224  Sum of the sixteenth powers of the first n primes.
 * https://oeis.org/A236224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} prime(k)^16
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Raise a seq_t value to the 16th power (iterative multiplication). */
static seq_t pow16(seq_t p)
{
    seq_t r = 1;
    for (int i = 0; i < 16; ++i)
        r *= p;
    return r;
}

/* Static storage for generated primes. */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t   prime_cnt  = 0;      /* number of primes stored */
static size_t   prime_cap  = 0;      /* allocated capacity */

/* Grow prime_list to accommodate ‘needed’ elements. */
static void grow_prime_list(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    uint64_t *new_ptr = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    assert(new_ptr != NULL);
    prime_list = new_ptr;
    prime_cap  = new_cap;
}

/* Static storage for accumulated sums a(i+1). */
static seq_t *sum_cache = NULL;      /* array of a(i) values */
static size_t sum_cache_len = 0;     /* number of sums stored */
static size_t sum_cache_cap = 0;     /* allocated capacity */

/* Grow sum_cache to accommodate ‘needed’ elements. */
static void grow_sum_cache(size_t needed)
{
    if (needed <= sum_cache_cap)
        return;
    size_t new_cap = (sum_cache_cap == 0) ? 16 : sum_cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    assert(new_ptr != NULL);
    sum_cache = new_ptr;
    sum_cache_cap = new_cap;
}

/* Simple trial‑division primality test using already known primes. */
static int is_prime(uint64_t x)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return 0;
    }
    return 1;
}

/* Generate and append the next prime after the current list. */
static void generate_next_prime(void)
{
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    for (;; ++candidate) {
        if (candidate < 2)
            continue;
        if (is_prime(candidate)) {
            grow_prime_list(prime_cnt + 1);
            prime_list[prime_cnt++] = candidate;
            return;
        }
    }
}

/* Return a(n) for n ≥ 1.  The function caches intermediate results
 * so that calls in any order are efficient. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, harmless placeholder */

    /* Ensure the cache can hold the requested index. */
    grow_sum_cache((size_t)n);

    /* Compute any missing primes and sums up to n. */
    for (size_t i = sum_cache_len; i < (size_t)n; ++i) {
        /* Ensure prime_list[i] exists. */
        while (prime_cnt <= i)
            generate_next_prime();

        seq_t p16 = pow16((seq_t)prime_list[i]);
        seq_t prev = (i == 0) ? 0 : sum_cache[i - 1];
        sum_cache[i] = prev + p16;
    }

    sum_cache_len = n;
    return sum_cache[n - 1];
}

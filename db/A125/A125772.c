/*
 * A125772  Primes not of the form j*T_k +/- 1, where T_k is the k-th
 *          triangular number greater than 9.
 * https://oeis.org/A125772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A125772: primes that are NOT of the form
   j*T_k ± 1 for any integer j ≥ 1 and any triangular number
   T_k > 9 (where T_k = k(k+1)/2).  The algorithm generates primes
   incrementally and tests each one by checking divisibility
   of p-1 and p+1 by all triangular numbers up to p+1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* External types provided by the build environment */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Static workspace ------------------------------------------------- */
/* ------------------------------------------------------------------ */

/* Cache for computed sequence terms */
static seq_t *cache_arr = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Array of primes discovered so far */
static uint64_t *primes_arr = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;
static uint64_t primes_limit = 1; /* last number tested for primality */

/* Position of the next unexamined prime in primes_arr */
static size_t prime_pos = 0;

/* Triangular numbers > 9 */
static uint64_t *triangular_arr = NULL;
static size_t triangular_len = 0;
static size_t triangular_cap = 0;
static uint64_t triangular_k    = 4;  /* current k for next triangular number */
static uint64_t triangular_next = 10; /* next triangular number */

/* ------------------------------------------------------------------ */
/*  Helper functions ------------------------------------------------- */
/* ------------------------------------------------------------------ */

static void cache_ensure(size_t newcap)
{
    if (cache_cap >= newcap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 4;
    if (cap < newcap) cap = newcap;
    cache_arr = (seq_t *)realloc(cache_arr, cap * sizeof(seq_t));
    cache_cap = cap;
}

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) cache_ensure(cache_cap + 1);
    cache_arr[cache_len++] = val;
}

static void add_prime(uint64_t p)
{
    if (primes_len == primes_cap) {
        size_t cap = primes_cap ? primes_cap * 2 : 4;
        primes_arr = (uint64_t *)realloc(primes_arr, cap * sizeof(uint64_t));
        primes_cap = cap;
    }
    primes_arr[primes_len++] = p;
}

/* Ensure triangular numbers are present up to 'bound' (inclusive) */
static void triangular_ensure(uint64_t bound)
{
    while (triangular_next <= bound) {
        if (triangular_len == triangular_cap) {
            size_t cap = triangular_cap ? triangular_cap * 2 : 4;
            triangular_arr = (uint64_t *)realloc(triangular_arr,
                                                 cap * sizeof(uint64_t));
            triangular_cap = cap;
        }
        triangular_arr[triangular_len++] = triangular_next;
        ++triangular_k;
        triangular_next = (uint64_t)(((unsigned __int128)triangular_k *
                                      (triangular_k + 1)) / 2);
    }
}

/* Check whether a prime is excluded by the definition */
static bool excluded_prime(uint64_t prime)
{
    triangular_ensure(prime + 1);
    for (size_t i = 0; i < triangular_len; ++i) {
        uint64_t T = triangular_arr[i];
        if ((prime - 1) % T == 0 || (prime + 1) % T == 0)
            return true;
    }
    return false;
}

/* Generate all primes up to 'limit' (inclusive) */
static void generate_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit) return;
    for (uint64_t num = primes_limit + 1; num <= limit; ++num) {
        bool prime_flag = true;
        uint64_t sqrt_n = (uint64_t)sqrt((long double)num);
        for (size_t i = 0; i < primes_len && primes_arr[i] <= sqrt_n; ++i) {
            if (num % primes_arr[i] == 0) {
                prime_flag = false;
                break;
            }
        }
        if (prime_flag) add_prime(num);
    }
    primes_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function ----------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1 */

    /* Generate terms until the cache has at least n entries */
    while (cache_len < (size_t)n) {
        /* Ensure we have at least one more prime to examine */
        if (prime_pos >= primes_len) {
            uint64_t new_limit = primes_limit * 2;
            generate_primes_up_to(new_limit);
        }
        uint64_t prime = primes_arr[prime_pos++];
        if (!excluded_prime(prime))
            cache_append((seq_t)prime);
    }
    return cache_arr[n - 1];
}

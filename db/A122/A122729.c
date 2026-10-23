/*
 * A122729  Primes that are the sum of 5 positive cubes.
 * https://oeis.org/A122729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime that is the sum of five positive cubes
 * Range:     u8 n <= 19, u16 n <= 5985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global caches (static to the translation unit). */

static uint64_t *prime_cache = NULL;   /* sequence values */
static size_t prime_cache_cap = 0;
static size_t prime_cache_len = 0;

static uint8_t *sieve = NULL;          /* primality sieve */
static uint64_t sieve_limit = 0;
static uint64_t last_checked = 1;      /* last number processed in the sieve */

static uint64_t *cubes = NULL;         /* list of cubes <= current limit */
static size_t cubes_cap = 0;
static size_t cubes_len = 0;

static uint64_t *sum2_vals = NULL;     /* sorted unique sums of two cubes */
static size_t sum2_cap = 0;
static size_t sum2_len = 0;

static uint8_t *sum3 = NULL;           /* boolean array: sum of three cubes */

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* Build the list of cubes <= limit. */
static void compute_cubes(uint64_t limit)
{
    if (cubes_cap == 0) {
        cubes_cap = 16;
        cubes = (uint64_t *)malloc(cubes_cap * sizeof(uint64_t));
    }
    cubes_len = 0;
    for (uint64_t i = 1; ; ++i) {
        uint64_t c = i * i * i;
        if (c > limit)
            break;
        if (cubes_len == cubes_cap) {
            cubes_cap <<= 1;
            cubes = (uint64_t *)realloc(cubes, cubes_cap * sizeof(uint64_t));
        }
        cubes[cubes_len++] = c;
    }
}

/* Build the list of sums of two cubes (unordered). */
static void compute_sum2(uint64_t limit)
{
    if (sum2_cap == 0) {
        sum2_cap = 32;
        sum2_vals = (uint64_t *)malloc(sum2_cap * sizeof(uint64_t));
    }
    sum2_len = 0;
    for (size_t i = 0; i < cubes_len; ++i) {
        for (size_t j = i; j < cubes_len; ++j) {
            uint64_t s = cubes[i] + cubes[j];
            if (s > limit)
                break;
            if (sum2_len == sum2_cap) {
                sum2_cap <<= 1;
                sum2_vals = (uint64_t *)realloc(sum2_vals,
                                                sum2_cap * sizeof(uint64_t));
            }
            sum2_vals[sum2_len++] = s;
        }
    }
    qsort(sum2_vals, sum2_len, sizeof(uint64_t), cmp_uint64);
}

/* Build the boolean array sum3[0..limit] where sum3[x] == 1 iff
   x can be written as the sum of three positive cubes. */
static void compute_sum3(uint64_t limit)
{
    free(sum3);
    sum3 = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    for (size_t i = 0; i < cubes_len; ++i) {
        uint64_t a = cubes[i];
        if (a > limit)
            break;
        for (size_t j = i; j < cubes_len; ++j) {
            uint64_t b = a + cubes[j];
            if (b > limit)
                break;
            for (size_t k = j; k < cubes_len; ++k) {
                uint64_t s = b + cubes[k];
                if (s > limit)
                    break;
                sum3[s] = 1;
            }
        }
    }
}

/* Build the primality sieve up to the given limit. */
static void compute_sieve(uint64_t limit)
{
    free(sieve);
    sieve = (uint8_t *)malloc(limit + 1);
    for (uint64_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    if (limit >= 0)
        sieve[0] = 0;
    if (limit >= 1)
        sieve[1] = 0;
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                sieve[m] = 0;
        }
    }
}

/* Append a prime to the sequence cache. */
static void push_cache(uint64_t p)
{
    if (prime_cache_len == prime_cache_cap) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap << 1 : 16;
        prime_cache = (uint64_t *)realloc(prime_cache,
                                          new_cap * sizeof(uint64_t));
        prime_cache_cap = new_cap;
    }
    prime_cache[prime_cache_len++] = p;
}

/* Ensure that the cache contains at least n terms of the sequence. */
static void ensure_cache(idx_t n)
{
    while (prime_cache_len < n) {
        if (sieve_limit == 0)
            sieve_limit = 1024;
        else
            sieve_limit <<= 1;                 /* double each time */

        compute_sieve(sieve_limit);
        compute_cubes(sieve_limit);
        compute_sum2(sieve_limit);
        compute_sum3(sieve_limit);

        for (uint64_t num = last_checked + 1; num <= sieve_limit; ++num) {
            if (!sieve[num])
                continue;
            if (num < 5)            /* smallest representable number */
                continue;

            bool found = false;
            for (size_t idx = 0; idx < sum2_len; ++idx) {
                uint64_t s2 = sum2_vals[idx];
                if (s2 >= num)
                    break;
                uint64_t rem = num - s2;
                if (rem <= sieve_limit && sum3[rem]) {
                    found = true;
                    break;
                }
            }
            if (found)
                push_cache(num);
        }
        last_checked = sieve_limit;
    }
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_cache(n);
    return (seq_t)prime_cache[(size_t)(n - 1)];
}

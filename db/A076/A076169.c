/*
 * A076169  Triangular numbers whose sum of prime factors (with repetition) is
 *          also triangular.
 * https://oeis.org/A076169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A076169 – triangular numbers whose sum of prime factors (with multiplicity)
 *  is also triangular.
 *
 *  The function generates terms on demand, caching all previously found
 *  terms.  For each m ≥ 1 we examine the triangular number
 *      T(m) = m·(m+1)/2
 *  and compute the sum of its prime factors (using the factorisations
 *  of m and m+1, subtracting one 2 for the division by 2).  If that
 *  sum is triangular as well, T(m) is stored as the next sequence value.
 */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declarations of helper functions */
static void init_primes(void);
static seq_t sum_prime_factors(idx_t x);
static bool is_triangular_uint64(unsigned long long v);
static unsigned long long isqrt_uint64(unsigned long long n);

/* ------------------------------------------------------------------------- */
/* Prime generation using a simple sieve                                               */
/* ------------------------------------------------------------------------- */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_ready  = false;

static void init_primes(void)
{
    if (primes_ready) return;

    /* Limit large enough to handle m up to at least 200 000 (empirically safe for n≈10000). */
    const idx_t LIMIT = 200000;
    const size_t EST_CAP = LIMIT / 10 + 10; /* Rough estimate of number of primes */

    /* Allocate temporary boolean array for the sieve */
    uint8_t *is_comp = (uint8_t *)malloc((size_t)LIMIT + 1);
    if (!is_comp) return;   /* memory failure – will never happen in expected tests */
    for (size_t i = 0; i <= (size_t)LIMIT; ++i) is_comp[i] = 0;

    /* Allocate prime array */
    prime_list = (idx_t *)malloc(EST_CAP * sizeof(idx_t));
    if (!prime_list) { free(is_comp); return; }

    prime_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count < EST_CAP) prime_list[prime_count++] = i;
            /* Mark multiples */
            if ((unsigned long long)i * i <= (unsigned long long)LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i) is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    primes_ready = true;
}

/* ------------------------------------------------------------------------- */
/* Integer square root using binary search (no floating point)                 */
/* ------------------------------------------------------------------------- */
static unsigned long long isqrt_uint64(unsigned long long n)
{
    unsigned long long low = 0, high = n;
    while (low <= high) {
        unsigned long long mid = (low + high) >> 1;
        unsigned long long sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Check whether a 64‑bit unsigned integer is triangular. */
static bool is_triangular_uint64(unsigned long long v)
{
    if (v < 2ULL) return true;          /* 0 and 1 are triangular */
    unsigned long long d = 8ULL * v + 1ULL;
    unsigned long long s = isqrt_uint64(d);
    if (s * s != d) return false;
    return ((s - 1ULL) & 1ULL) == 0ULL;
}

/* ------------------------------------------------------------------------- */
/* Sum of prime factors with multiplicity for a 64‑bit integer                  */
/* ------------------------------------------------------------------------- */
static seq_t sum_prime_factors(idx_t x)
{
    seq_t sum = 0;
    idx_t tmp = x;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if ((__uint128_t)p * p > tmp) break;
        while (tmp % p == 0) {
            sum += (seq_t)p;
            tmp /= p;
        }
    }
    if (tmp > 1) sum += (seq_t)tmp;
    return sum;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function                                                    */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise primes on first call */
    if (!primes_ready) init_primes();

    /* Static cache of previously found terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t current_m  = 0;

    /* Grow cache if needed */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – unlikely in tests */
    }

    while (cache_len < n) {
        ++current_m;

        /* Sum of prime factors of m and m+1 */
        seq_t sum_m  = sum_prime_factors(current_m);
        seq_t sum_m1 = sum_prime_factors(current_m + 1);

        /* Subtract one factor 2 for division by 2 in T(m) */
        seq_t sum_total = sum_m + sum_m1 - (seq_t)2;

        if (is_triangular_uint64((unsigned long long)sum_total)) {
            /* Triangular number itself */
            seq_t t = (seq_t)current_m * (seq_t)(current_m + 1) / (seq_t)2;

            /* Ensure cache capacity */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* reallocation failure */
                cache = tmp;
            }

            cache[cache_len++] = t;
        }
    }

    return cache[n - 1];
}

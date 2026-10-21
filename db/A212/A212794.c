/*
 * A212794  Triangular numbers (A000217) which are also hypotenuse numbers
 *          (A009003).
 * https://oeis.org/A212794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth triangular number that is also a hypotenuse. * A
 *            positive integer is a hypotenuse of a right–triangle iff it has
 *            * at least one prime divisor ≡ 1 (mod 4). This implementation *
 *            enumerates triangular numbers Tₖ = k(k+1)/2 and accepts the ones
 *            * that contain such a prime factor. The resu...
 * Range:     u8 n <= 12, u16 n <= 288, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ----------------------------------------------------------------- */
/* Prime sieve ----------------------------------------------------- */
/* ----------------------------------------------------------------- */
#define PRIME_LIMIT 2000000ULL   /* large enough to cover all required primes */

static bool *sieve        = NULL;  /* composite marks */
static uint64_t *prime_list = NULL;/* primes */
static size_t   prime_cnt   = 0;    /* number of primes */

static void init_primes(void)
{
    if (sieve != NULL) return; /* already initialised */

    sieve = (bool*)malloc((size_t)(PRIME_LIMIT + 1) * sizeof(bool));
    if (sieve == NULL) return; /* allocation failure is fatal */

    /* assume prime */
    for (size_t i = 2; i <= (size_t)PRIME_LIMIT; ++i)
        sieve[i] = false;

    for (size_t i = 2; i * i <= (size_t)PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= (size_t)PRIME_LIMIT; j += i)
                sieve[j] = true;
        }
    }

    /* count primes */
    for (size_t i = 2; i <= (size_t)PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_cnt++;

    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (prime_list == NULL) return; /* allocation failure */

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = (uint64_t)i;
}

/* Test whether n has a prime divisor ≡ 1 (mod 4) */
static bool has_one_mod4_prime(uint64_t n)
{
    if (n < 5) return false; /* 1,2,3,4 have no such factor */

    init_primes();

    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > m / p) break;          /* p > sqrt(m) */
        if (m % p == 0) {
            if (p % 4 == 1) return true;
            while (m % p == 0) m /= p;
        }
    }
    /* Remaining factor m > sqrt(original n) */
    return (m > 1 && (m % 4 == 1));
}

/* ----------------------------------------------------------------- */
/* Sequence cache ---------------------------------------------------- */
/* ----------------------------------------------------------------- */
static seq_t *cache   = NULL;   /* cached sequence values */
static size_t cache_sz = 0;     /* number of cached terms   */
static size_t cache_cap = 0;    /* allocated capacity       */
static idx_t  next_k   = 1;     /* next triangular index to test */

/* Grow the cache array as needed. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_sz >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* ----------------------------------------------------------------- */
/* Main function ----------------------------------------------------- */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n = 0 in OEIS terms */

    /* Generate terms until we have at least n of them. */
    while (cache_sz < (size_t)n) {
        /* Triangular number Tₖ = k(k+1)/2 */
        seq_t T = (seq_t)next_k * ((seq_t)next_k + 1) / 2;

        /* Split k(k+1)/2 into two coprime factors. */
        uint64_t a_part, b_part;
        if (next_k % 2 == 0) {
            a_part = next_k / 2;
            b_part = next_k + 1;
        } else {
            a_part = next_k;
            b_part = (next_k + 1) / 2;
        }

        /* Check hypotenuse property. */
        if (has_one_mod4_prime(a_part) || has_one_mod4_prime(b_part)) {
            grow_cache();
            cache[cache_sz++] = T;
        }
        ++next_k;
    }
    return cache[n - 1];
}

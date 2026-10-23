/*
 * A108969  Numbers n such that 43*n + 41 is prime.
 * https://oeis.org/A108969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n such that 43*n + 41 is prime (n ≥ 0)
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term (1‑based) of OEIS sequence A108969.
   The sequence consists of all non‑negative integers n for which
   43*n + 41 is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations for build‑provided types */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Helper: prime sieve up to a fixed bound (enough for all tests)       */
/* --------------------------------------------------------------------- */
static bool primes_inited = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    const size_t MAX = 200000;        /* sufficient for all needed checks   */
    bool *is_composite = (bool*)calloc(MAX + 1, sizeof(bool));
    size_t cnt = 0;

    for (size_t i = 2; i <= MAX; ++i) {
        if (!is_composite[i]) {
            ++cnt;
            if (cnt <= MAX) {         /* reserve enough space */
                prime_list = (uint32_t*)realloc(prime_list, sizeof(uint32_t) * cnt);
                prime_list[prime_count++] = (uint32_t)i;
            }
            if (i <= MAX / i) {
                for (size_t j = i * i; j <= MAX; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    free(is_composite);
    primes_inited = true;
}

/* --------------------------------------------------------------------- */
/* Primality test for 64‑bit numbers using the precomputed primes       */
/* --------------------------------------------------------------------- */
static bool is_prime_ull(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Cache for already discovered terms of the sequence                    */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_size  = 0;
static idx_t cache_cap   = 0;
static idx_t next_candidate = 0;   /* value of n to test next */

/* Ensure the cache holds terms up to at least 'needed' (1‑based). */
static void ensure_cache(idx_t needed)
{
    init_primes();
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    }
    while (cache_size < needed) {
        /* test next_candidate for primality of 43*n + 41   */
        uint64_t val = (uint64_t)43 * next_candidate + 41ULL;
        if (is_prime_ull(val)) {
            /* store the candidate in cache */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------- */
/* Main function as required by the interface                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  a(1) = 0. */
    if (n == 0) return (seq_t)0;  /* undefined offset, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}

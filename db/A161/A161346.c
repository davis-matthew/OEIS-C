/*
 * A161346  a(n) = A161345(n)/3.
 * https://oeis.org/A161346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 6544, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161346: a(n) = the nth element of the union of {4,6,9} and all odd primes */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* array of precomputed terms */
static size_t cache_len = 0;    /* number of terms stored */
static idx_t next_candidate = 3;/* next integer to test */

static idx_t *prime_list = NULL;/* list of primes for trial division */
static size_t prime_count = 0;   /* number of primes in the list */

/* Ensure the prime_list can hold at least `need` primes */
static void grow_prime_list(size_t need)
{
    size_t new_cap = prime_count ? prime_count << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    prime_list = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
}

/* Trial‑division primality test using the primes gathered so far. */
static bool is_prime(idx_t v)
{
    if (v < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p * p > v) break;
        if (v % p == 0) return false;
    }
    return true;
}

/* Build the sequence containing the first `need` terms. */
static void build_cache(idx_t need)
{
    /* Allocate storage for the sequence itself. */
    cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
    cache_len = 0;
    next_candidate = 3;

    /* Allocate prime list and seed it with 2 and 3. */
    prime_list = (idx_t*)realloc(prime_list, 16 * sizeof(idx_t));
    prime_count = 0;
    prime_list[prime_count++] = 2;
    prime_list[prime_count++] = 3;

    /* Generate terms until we have `need` of them. */
    while (cache_len < need) {
        idx_t val = next_candidate++;

        /* Add the exceptional numbers 4, 6, and 9. */
        if (val == 4 || val == 6 || val == 9) {
            cache[cache_len++] = (seq_t)val;
            continue;
        }

        /* Add odd primes. */
        if ((val & 1U) && is_prime(val)) {
            cache[cache_len++] = (seq_t)val;
            /* Store the new prime for future tests. */
            grow_prime_list(prime_count + 1);
            prime_list[prime_count++] = val;
        }
    }
}

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    if (cache_len < n) build_cache(n);   /* extend cache if needed */
    return cache[n - 1];
}

/*
 * A138117  Triangle read by rows: row n lists the first 2n-1 prime numbers.
 * https://oeis.org/A138117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   let r = ceil(sqrt(n)). Then a(n) = the (n-(r-1)²)-th prime
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the flattened triangle where row r contains the first
 * 2r-1 prime numbers.  Because the total number of elements up to row r
 * equals r², we have r = ceil(sqrt(n)).  Inside that row the element
 * positions 1..(2r-1) correspond to the prime sequence p₁, p₂, … .
 * Hence a(n)=p_k with k = n-(r-1)².  The implementation below
 * generates primes on demand using a simple incremental sieve. */

#include <stdint.h>
#include <stdlib.h>

/* Cache of generated primes as seq_t.  The array is grown to the
 * required size on demand.  Since the largest prime needed for
 * n ≤ 10 000 is the 199th prime (1223), the cache stays tiny. */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;   /* number of primes stored */
static size_t prime_cache_cap = 0;   /* current capacity of prime_cache */
static uint32_t last_candidate = 2;  /* next odd number to test for primality */

/* Ensure at least `need` primes are generated and stored in prime_cache.
 * Returns 0 on allocation failure, otherwise 1.  The function can be
 * called repeatedly; it will generate more primes only if required. */
static int ensure_primes(size_t need)
{
    if (prime_cache_len >= need)
        return 1;

    while (prime_cache_len < need) {
        /* Grow the array if necessary */
        if (prime_cache_len == prime_cache_cap) {
            size_t new_cap = prime_cache_cap == 0 ? 64 : prime_cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed; caller will return 0 */
                return 0;
            }
            prime_cache = tmp;
            prime_cache_cap = new_cap;
        }

        /* Test last_candidate for primality */
        uint32_t candidate = last_candidate;
        int is_prime = 1;
        for (size_t i = 0; i < prime_cache_len; ++i) {
            uint32_t p = (uint32_t)prime_cache[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            prime_cache[prime_cache_len++] = (seq_t)candidate;
        }

        last_candidate = candidate + 1;
        if (last_candidate == 2)  /* overflow (unlikely) */
            last_candidate = 3;
    }
    return 1;
}

/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for offset 0, return 0 */

    /* Find row r = ceil(sqrt(n)).  For small n a simple loop is fine. */
    idx_t r = 1;
    while (r * r < n)
        ++r;

    /* Number of primes needed for this row */
    size_t need = 2 * (size_t)r - 1;

    if (!ensure_primes(need))
        return (seq_t)0;         /* allocation failed */

    /* Position within the row (0‑based) */
    idx_t prev = (r - 1) * (r - 1);
    idx_t pos = n - prev - 1;    /* 0 ≤ pos < need */

    return prime_cache[(size_t)pos];
}

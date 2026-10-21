/*
 * A227794  Primes of the form floor(Pi*k^2).
 * https://oeis.org/A227794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form floor(Pi * k^2) for some
 *            integer k ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 1500, u128 n <= 1500,
 *            big any n
 * Helpers:   alloc_seq, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility headers that are available in the build environment. */
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"

/* High‑precision rational approximation of π.
   3.141592653589793 = 3141592653589793 / 1000000000000000 */
static const seq_t PI_NUM = 3141592653589793ULL;
static const seq_t PI_DEN = 1000000000000000ULL;

/* Cache of primes of the required form. */
static seq_t *prime_cache = NULL;   /* dynamic array of seq_t */
static idx_t cache_cap = 0;         /* current capacity of prime_cache */
static idx_t cache_len = 0;         /* number of stored primes */
static idx_t next_k = 1;            /* next k to test */

/* Compute floor(π * k^2) using a 128‑bit intermediate to avoid overflow
   on 64‑bit systems.  The result is returned as a seq_t. */
static seq_t floor_pi_sq(idx_t k)
{
    unsigned __int128 kk   = (unsigned __int128)k * k;                      /* k²   */
    unsigned __int128 prod = kk * (unsigned __int128)PI_NUM;                /* num·k² */
    unsigned __int128 quot = prod / (unsigned __int128)PI_DEN;             /* floor division */
    return (seq_t)quot;
}

/*
 * Return the n-th term of A227794.
 *
 * The function builds up a cache of primes of the form floor(π*k^2) lazily.
 * Calls may occur in any order; the cache grows only as needed.
 */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is 1‑based; return 0 for safety */
        return (seq_t)0;

    /* Initialise cache on first call. */
    if (prime_cache == NULL) {
        prime_cache = alloc_seq(16);
        cache_cap   = 16;
        cache_len   = 0;
    }

    /* Generate primes until we have at least n of them. */
    while (cache_len < n) {
        seq_t val = floor_pi_sq(next_k);
        if (is_prime_big(val)) {
            if (cache_len == cache_cap)
                grow_seq(&prime_cache, &cache_cap, cache_len + 1);
            prime_cache[cache_len++] = val;
        }
        ++next_k;
    }

    return prime_cache[n - 1];
}

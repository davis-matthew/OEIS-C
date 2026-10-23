/*
 * A169643  Numbers k such that neither composite(k)+-1 is composite.
 * https://oeis.org/A169643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th index k of a composite number m = composite(k)
 *            such that m-1 and m+1 are both prime. Both sides are interpreted
 *            as integers, not composites. The function computes the sequence
 *            lazily, caching results between calls using static storage
 * Range:     u8 n <= 20, u16 n <= 941, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enlarge_uint64_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/enlarge_uint64_arr.h"

/* --------------------------------------------------------------------- */
/* Dynamic array helpers (uint64_t version) ---------------------------- */

/* --------------------------------------------------------------------- */
/* Prime list ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */
static uint64_t *primes = NULL;
static size_t primes_cp = 0, primes_size = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    if (primes_initialized) return;
    enlarge_uint64_arr(&primes, &primes_size, 16);
    primes[0] = 2;
    primes[1] = 3;
    primes_cp = 2;
    primes_initialized = true;
}

static void add_prime(uint64_t p) {
    enlarge_uint64_arr(&primes, &primes_size, primes_cp + 1);
    primes[primes_cp++] = p;
}

/* --------------------------------------------------------------------- */
/* Primality test (trial division using current prime list) ----------- */
/* --------------------------------------------------------------------- */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    init_primes();
    for (size_t i = 0; i < primes_cp; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* p*p > n     */
        if (n % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Cache for the sequence ----------------------------------------------*/
/* --------------------------------------------------------------------- */
static uint64_t *cache = NULL;
static size_t cache_cap = 0, cache_size = 0;

/* --------------------------------------------------------------------- */
/* State for incremental generation -------------------------------------*/
/* --------------------------------------------------------------------- */
static uint64_t next_candidate = 4;           /* next integer to examine   */
static uint64_t composite_index = 0;          /* current index in composite list */

/* --------------------------------------------------------------------- */
/* Ensure that cache contains at least n elements --------------------- */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n) {
    while (cache_size < (size_t)n) {
        uint64_t x = next_candidate;
        if (!is_prime_u64(x)) {                 /* x is composite */
            ++composite_index;                  /* composite_index == k of this composite */
            if (is_prime_u64(x - 1) && is_prime_u64(x + 1)) {
                enlarge_uint64_arr(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = composite_index;
            }
        } else {
            add_prime(x);                       /* keep primes for future tests */
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------- */
/* Public function ------------------------------------------------------*/
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0; return 0 */
    ensure_cache(n);
    return (seq_t)cache[(size_t)(n - 1)];
}

/*
 * A096983  a(n) is the smallest prime whose base-n digits add up to a
 *          composite number.
 * https://oeis.org/A096983
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_sum_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function computes the OEIS sequence A096983:
 * a(n) = smallest prime whose base-n digit sum is a composite number.
 * The sequence is defined for n >= 2 (offset 2).
 *
 * The implementation performs a straightforward search for each n:
 *   * It starts at the prime number 2 and tests successive odd integers.
 *   * For each prime p, it computes the digit sum of p in base n.
 *     If that sum is composite (i.e., > 1 and not prime), p is returned.
 *
 * A cache is used so that repeated calls with the same n do not repeat the
 * search.  The search is reasonably fast for n up to about 10 000, as
 * required by the performance constraints.
 *
 * Helper functions:
 *   • is_prime_u64 : Deterministic Miller‑Rabin primality test for 64‑bit
 *     integers, using the standard set of bases.
 *   • digit_sum_in_base : Computes the digit sum of a number in an arbitrary base.
 *   • is_composite : Checks whether an unsigned integer is composite.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum_in_base.h"

/* ------------------------ Auxiliary helpers ------------------------ */

/* Modular multiplication, safe for 64‑bit operands. */
static uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* Modular exponentiation (base^exp mod mod).  Assumes mod > 1. */
static uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod_u64(result, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin test for 64‑bit integers. */
static int is_prime_u64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return 1;
        if (n % small_primes[i] == 0) return 0;
    }

    /* Write n-1 as d * 2^s with d odd. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int continue_outer = 0;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) { continue_outer = 1; break; }
        }
        if (continue_outer) continue;
        return 0;   /* Composite */
    }
    return 1;       /* Probably prime, but deterministic here */
}

/* Test whether n is composite (i.e., n > 1 and not prime). */
static int is_composite(uint64_t n)
{
    if (n <= 3) return 0;          /* 0,1,2,3 are not composite */
    for (uint64_t i = 2; i * i <= n; ++i)
        if (n % i == 0) return 1;
    return 0;
}

/* ------------------------- Main sequence function ------------------- */

static seq_t *cache = NULL;     /* Cached results, indexed by n */
static idx_t cache_size = 0;    /* Current size of the cache array */

/* Resize cache to accommodate indices up to 'needed'.  New entries are
 * initialized to 0, which indicates an uncomputed value. */
static void resize_cache(idx_t needed)
{
    if (needed < cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 64;
    while (new_size <= needed) new_size *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_ptr) return;           /* allocation failure: give up */
    /* Initialize new entries to 0. */
    if (new_ptr != cache) cache = new_ptr;
    memset(cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    cache_size = new_size;
}

/* The public function requested by the template. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;       /* undefined for n < 2 */

    /* Ensure cache has room for n. */
    resize_cache(n);

    if (cache[n] != 0) return cache[n];   /* already computed */

    /* ---- Search for the smallest prime satisfying the condition ---- */
    uint64_t result = 0;

    /* Check prime 2 separately (since we will skip even numbers). */
    if (n >= 2) {
        uint64_t sum = digit_sum_in_base(2, (uint64_t)n);
        if (is_composite(sum)) result = 2;
    }

    /* If not found, search odd primes starting from 3. */
    if (result == 0) {
        for (uint64_t p = 3; ; p += 2) {
            if (!is_prime_u64(p)) continue;
            uint64_t sum = digit_sum_in_base(p, (uint64_t)n);
            if (is_composite(sum)) {
                result = p;
                break;
            }
            /* Prevent infinite loop in pathological cases */
            if (p == UINT64_MAX) break;
        }
    }

    /* Store and return the result. */
    cache[n] = (seq_t)result;
    return cache[n];
}

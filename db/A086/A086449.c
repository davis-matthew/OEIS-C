/*
 * A086449  a(0) = 1, a(2n+1) = a(n), a(2n) = a(n) + a(n-1) + ... + a(n-2^m) +
 *          ... where a(n) = 0 for n < 0.
 * https://oeis.org/A086449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   a(0) = 1 * a(2n+1) = a(n) * a(2n) = a(n) + Σ_{k=0}^{⌊log₂n⌋}
 *            a(n-2ᵏ) (n ≥ 0, a(m)=0 for m<0) * (The summation includes a(n)
 *            when k=0.) * Recurrence uses memoization; values are computed
 *            lazily with static cache. * * Implementation note: * The cache
 *            is an array of seq_t, indexed by idx_t. The arra...
 * Range:     u8 n <= 79, u16 n <= 975, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External types supplied by the build environment. */
typedef uint64_t idx_t;

/* seq_t is supplied by the macros of the build system; we only use
 * arithmetic on it.  The implementation below performs operations
 * solely on seq_t, idx_t, and standard arithmetic types. */
extern int dummy; /* dummy to silence unused warning if not needed */

/* static cache for memoization */
static seq_t *cache = 0;
static idx_t cache_size = 0;

/* Ensure that the cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n < cache_size)
        return;
    /* Grow to at least 2×(n+1) or at least 1. */
    idx_t new_size = cache_size == 0 ? 1 : cache_size;
    while (new_size <= n)
        new_size <<= 1; /* double until large enough */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed: terminate. */
        exit(1);
    }
    /* Initialize the new elements to 0 (sentinel). */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

/* Forward declaration for recursion. */
static seq_t a_val(idx_t n);

/* Public function as required by the build system. */
seq_t a(idx_t n) {
    return a_val(n);
}

/* Recursive implementation with memoization. */
static seq_t a_val(idx_t n) {
    if (n == 0)
        return 1; /* base case */
    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];
    seq_t result;
    if (n & 1U) { /* odd: a(2m+1) = a(m) */
        result = a_val(n >> 1);
    } else { /* even: a(2m) = a(m) + Σ_{k=0}^{⌊log₂m⌋} a(m-2ᵏ) */
        idx_t m = n >> 1;
        seq_t sum = 0;
        /* first term a(m) */
        sum += a_val(m);
        /* add a(m - 2ᵏ) for powers of two up to m */
        for (idx_t pow = 1; pow <= m; pow <<= 1) {
            sum += a_val(m - pow);
        }
        result = sum;
    }
    cache[n] = result;
    return result;
}

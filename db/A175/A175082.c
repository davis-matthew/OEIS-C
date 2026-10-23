/*
 * A175082  Possible values for sum of perfect divisors of n.
 * https://oeis.org/A175082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complement of perfect powers >= 4: a(n) is the n-th positive integer that
 * is not a perfect power (>=4).  This is OEIS A175082.  The implementation
 * builds the sequence lazily, storing values in a static array and generating
 * new terms as needed. */

#include <stdint.h>
#include <stdlib.h>

/* Helper: compute floor of n^(1/k) for n >= 1, k >= 2.  Uses binary search
 * and __int128 for safe multiplication. */
static uint64_t kth_root_uint64(uint64_t n, unsigned int k)
{
    uint64_t low = 1, high = 1;
    /* find an upper bound for the root */
    while (1) {
        __int128 p = 1;
        for (unsigned int i = 0; i < k; ++i) {
            p *= high;
            if (p > n) break;
        }
        if (p > n) break;
        high <<= 1;
    }
    /* binary search within [low, high] */
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        __int128 p = 1;
        for (unsigned int i = 0; i < k; ++i) {
            p *= mid;
            if (p > n) break;
        }
        if (p <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Helper: test whether n is a perfect power >= 4. */
static int is_perfect_power(uint64_t n)
{
    if (n < 4)
        return 0;                       /* 1,2,3 are never counted */

    /* maximum possible exponent: floor(log2(n)) */
    unsigned int max_k = 0;
    uint64_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++max_k;
    }

    for (unsigned int k = 2; k <= max_k; ++k) {
        uint64_t r = kth_root_uint64(n, k);
        if (r < 2)
            break;
        __int128 p = 1;
        for (unsigned int i = 0; i < k; ++i)
            p *= r;
        if (p == n)
            return 1;                /* perfect power found */
    }
    return 0;
}

/* ------- lazy cache for the sequence ------- */

static seq_t *non_pow_cache = NULL;      /* array of previously computed terms */
static idx_t  cache_size = 0;             /* number of elements in cache */
static idx_t  cache_cap  = 0;             /* allocated capacity */
static uint64_t next_candidate = 1;       /* next integer to test */

/* ensure the cache has capacity at least 'need' */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while ((idx_t)new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(non_pow_cache, new_cap * sizeof(seq_t));
    non_pow_cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* ---------------- public function ---------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined; but avoid UB */

    /* generate terms until we have at least n in cache */
    while (cache_size < n) {
        if (!is_perfect_power(next_candidate)) {
            ensure_cache_capacity(cache_size + 1);
            non_pow_cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return non_pow_cache[n - 1];
}

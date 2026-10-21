/*
 * A060831  a(n) = Sum_{k=1..n} (number of odd divisors of k) (cf. A001227).
 * https://oeis.org/A060831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} number_of_odd_divisors(k)
 * Range:     u8 n <= 94, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(0) = 0 */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memset */

static seq_t   *a_cache  = NULL;   /* cumulative sums */
static uint64_t *oddcnt  = NULL;   /* odd divisor counts */
static size_t   cache_sz = 0;      /* cached array size (a_cache[0..cache_sz-1]) */

/* Extend caches up to index n (inclusive) */
static void extend(idx_t n)
{
    if (n < (idx_t)cache_sz) return;          /* already cached */

    size_t new_sz = (size_t)n + 1u;           /* new array length */

    /* reallocate storage */
    oddcnt = (uint64_t *)realloc(oddcnt, new_sz * sizeof(uint64_t));
    a_cache = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));

    /* initialise odd divisor counts */
    memset(oddcnt, 0, new_sz * sizeof(uint64_t));

    /* count odd divisors: only odd i contribute */
    for (idx_t i = 1; i <= n; i += 2u) {
        for (idx_t j = i; j <= n; j += i) {
            oddcnt[j]++;
        }
    }

    /* build prefix sums */
    a_cache[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        a_cache[i] = a_cache[i - 1] + (seq_t)oddcnt[i];
    }

    cache_sz = new_sz;
}

/* Public function required by the evaluator */
seq_t a(idx_t n)
{
    extend(n);
    return a_cache[(size_t)n];
}

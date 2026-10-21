/*
 * A075343  a(1) = 1, a(n+1) is the smallest number such that there are n
 *          composite numbers between a(n) and a(n+1) exclusive.
 * https://oeis.org/A075343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n≥2, a(n) is the smallest integer m such that there
 *            are exactly (n-1) composite numbers between a(n-1) and m
 *            exclusive. Equivalently, a(n)=c_(n‑1)+1 where c_k is the k‑th
 *            composite following a(n‑1)
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Simple primality test for values up to 2⁶⁴-1 (uint64_t). */
static bool is_prime_simple(idx_t n)
{
    if (n <= 3) return n > 1;          /* 2 and 3 are prime */
    if ((n & 1) == 0) return false;    /* even numbers >2 are composite */

    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* Cache for the sequence.  cache[0] holds a(1).  */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements stored (>=0) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Grow the cache so that it can hold at least n elements
   (indices 0 .. n-1).  The first element is already set to a(1)=1. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    /* Allocate or grow the array.  Double the capacity until it
       reaches n. */
    while (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms.  For i from cache_len to n-1:   */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i==0 corresponds to a(1), already stored.           */
        idx_t prev   = (idx_t)cache[i - 1];   /* a(i) is stored at i-1 */
        idx_t target = i;                     /* need the i‑th composite    */
        idx_t count  = 0;
        idx_t cand   = prev + 1;              /* start searching after prev */

        /* Find the target composite.  */
        while (true) {
            if (cand > 1 && !is_prime_simple(cand)) {
                ++count;
                if (count == target) break;
            }
            ++cand;
        }
        /* a(i+1) = the composite found + 1.              */
        cache[i] = (seq_t)(cand + 1);
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1, harmless */

    /* Initialise cache once.  */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();           /* out of memory */
        cache[0] = (seq_t)1;           /* a(1) = 1 */
        cache_len = 1;
    }

    ensure_cache(n);
    return cache[n - 1];               /* a(n) is stored at index n-1 */
}

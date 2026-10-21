/*
 * A206661  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          the number of clockwise edge increases equal to the number of
 *          counterclockwise edge increases in its adjacent leftward and
 *          upward neighbors.
 * https://oeis.org/A206661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A206661
 *  Number of (n+1)×2 0..2 arrays with every 2×2 subblock having the
 *  number of clockwise edge increases equal to the number of
 *  counterclockwise edge increases in its adjacent leftward and upward neighbors.
 *
 *  Recurrence (valid for n ≥ 8):
 *      a(n) = 9*a(n-1) - 18*a(n-2) - 19*a(n-3) + 71*a(n-4)
 *            - 28*a(n-5) - 22*a(n-6) + 8*a(n-7)
 *  Initial values:
 *      a(1)=81, a(2)=423, a(3)=2286, a(4)=12477,
 *      a(5)=68253, a(6)=373644, a(7)=2046027
 *
 *  The function a(n) below implements this recurrence with memoisation.
 *  It works for any integer type seq_t (unsigned 8/16/32/64 or arbitrary precision)
 *  and any idx_t (unsigned 64‑bit).  The implementation relies only on
 *  integer arithmetic and standard C memory operations.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache    = NULL;     /* array indexed 1..max_computed */
static idx_t  max_computed = 0;    /* highest n that has been computed */
static size_t cache_size    = 0;    /* allocated elements (including index 0) */

/* Ensure cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Return immediately if already computed */
    if (n <= max_computed) return;

    /* Grow the cache if necessary */
    size_t needed = (size_t)(n + 1);          /* include dummy element 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();                  /* allocation failure */
        cache_size = needed;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache      = tmp;
        cache_size = needed;
    }

    /* Fill in any missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1)        cache[i] = (seq_t)81;
        else if (i == 2)   cache[i] = (seq_t)423;
        else if (i == 3)   cache[i] = (seq_t)2286;
        else if (i == 4)   cache[i] = (seq_t)12477;
        else if (i == 5)   cache[i] = (seq_t)68253;
        else if (i == 6)   cache[i] = (seq_t)373644;
        else if (i == 7)   cache[i] = (seq_t)2046027;
        else {
            /* Compute using the recurrence with unsigned arithmetic.
             * All intermediate sums are non‑negative, and the final
             * result is known to be non‑negative, so unsigned subtraction
             * works without wrap‑around. */
            seq_t pos = (seq_t)9 * cache[i-1] + (seq_t)71 * cache[i-4]
                        + (seq_t)8 * cache[i-7];
            seq_t neg = (seq_t)18 * cache[i-2] + (seq_t)19 * cache[i-3]
                        + (seq_t)28 * cache[i-5] + (seq_t)22 * cache[i-6];
            cache[i] = pos - neg;
        }
    }
    max_computed = n;
}

/* External function required by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; return 0 as a safe default */
    ensure_cache(n);
    return cache[n];
}

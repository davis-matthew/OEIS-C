/*
 * A232602  a(n) = Sum_{k=0..n} k^p*q^k, where p=3, q=-2.
 * https://oeis.org/A232602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 18, i64 n <= 46, i128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=-2, a(2)=30, a(3)=-186, a(4)=838
   a(n) = -7*a(n-1)-16*a(n-2)-8*a(n-3)+16*a(n-4)+16*a(n-5) for n>=5.
   Values are memoised.  For integral seq_t (e.g. int64_t) a 128‑bit
   intermediate is used to avoid overflow; for arbitrary‑precision
   types the normal multiplication is used. */
#include <stdlib.h>

static inline seq_t mul_small(seq_t a, int m)
{
#ifdef __cplusplus
    /* In C++ builds the type may be an arbitrary‑precision integer. */
    return a * (seq_t)m;
#else
    /* In C the built‑in integer may overflow; use 128‑bit intermediate. */
    return (seq_t)((__int128)a * m);
#endif
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Grow the cache so that it can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len) return;
    seq_t *tmp = (seq_t *)realloc(cache,
                                  ((size_t)n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                      /* allocation failure */
    cache = tmp;
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Small indices – return the known constants directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)30;
    if (n == 3) return (seq_t)-186;
    if (n == 4) return (seq_t)838;

    /* Initialise the cache with the first five terms if not yet done. */
    if (cache_len < 5) {
        ensure_cache(4);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)-186;
        cache[4] = (seq_t)838;
    }

    /* If we already know the term, return it. */
    if (n < cache_len) return cache[n];

    /* Remember the current length; we will compute new values up to n. */
    idx_t start = cache_len;
    ensure_cache(n);                      /* grow the array as needed */

    for (idx_t i = start; i <= n; ++i) {
        seq_t val =
            mul_small(cache[i-1], -7) +
            mul_small(cache[i-2], -16) +
            mul_small(cache[i-3], -8) +
            mul_small(cache[i-4], 16) +
            mul_small(cache[i-5], 16);
        cache[i] = val;
    }

    return cache[n];
}

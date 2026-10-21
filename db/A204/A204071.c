/*
 * A204071  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A204071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204071
   Empirical recurrence (valid for n ≥ 5):
   a(n) = 20*a(n-1) – 73*a(n-2) + 86*a(n-3) – 32*a(n-4)
   Initial values:
   a(1)=188, a(2)=2882, a(3)=45056, a(4)=706454
   The function memoises results so that any order of calls returns
   the correct value.  It works for all compatible builds: fixed‑width
   unsigned integers (64‑bit) or arbitrary‑precision integer classes. */

#include <stdlib.h>   /* for realloc, exit */

#if defined(__cplusplus)
#include <cstddef>    /* for std::size_t */

/* Compute one term of the recurrence.  For integer types the
   calculation is performed with 128‑bit intermediates to avoid
   overflow; for non‑integer types (big‐integer classes) we use
   the normal operators. */
static inline seq_t _compute_term(seq_t a1, seq_t a2, seq_t a3, seq_t a4)
{
    if constexpr (sizeof(seq_t) <= 8) {               /* integer type */
        __int128 t = (__int128)a1 * 20
                     - (__int128)a2 * 73
                     + (__int128)a3 * 86
                     - (__int128)a4 * 32;
        return (seq_t)t;
    } else {                                          /* arbitrary‑precision type */
        seq_t t = a1 * 20;
        t -= a2 * 73;
        t += a3 * 86;
        t -= a4 * 32;
        return t;
    }
}
#else
/* C implementation – all arithmetic performed with unsigned 128‑bit
   intermediates and cast back to seq_t (uint64_t). */
static inline seq_t _compute_term(seq_t a1, seq_t a2, seq_t a3, seq_t a4)
{
    __uint128_t t = (__uint128_t)a1 * 20
                    - (__uint128_t)a2 * 73
                    + (__uint128_t)a3 * 86
                    - (__uint128_t)a4 * 32;
    return (seq_t)t;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe for out‑of‑range calls */

    static seq_t *cache = NULL;      /* memoised values 1 … */
    static idx_t cache_size = 0;     /* current size of cache (last index filled) */

    if (cache_size < n) {
        /* extend cache to hold indices up to n (index 0 unused) */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) { exit(EXIT_FAILURE); }
        cache = new_cache;

        /* initialise new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)188;
            else if (i == 2) cache[i] = (seq_t)2882;
            else if (i == 3) cache[i] = (seq_t)45056;
            else if (i == 4) cache[i] = (seq_t)706454;
            else
                cache[i] = _compute_term(cache[i-1], cache[i-2], cache[i-3], cache[i-4]);
        }
        cache_size = n;
    }
    return cache[n];
}

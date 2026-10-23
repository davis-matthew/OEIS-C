/*
 * A154144  Indices k such that 13 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - a(n-4) + 2 (n > 4) * with initial values
 *            a(1)=2, a(2)=8, a(3)=23, a(4)=53. * The function memoises
 *            computed terms in a static dynamic array. * Works for any seq_t
 *            (integer type or arbitrary‑precision class) and * any idx_t
 *            (uint64_t) in C11 or C++17
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL; /* cache of computed terms */
static size_t  cache_cap = 0;    /* allocated capacity */
static idx_t   cache_size = 0;   /* number of terms stored */

/* Ensure the cache can hold at least `need` entries. */
static void ensure_size(idx_t need)
{
    if (cache_cap >= (size_t)need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < (size_t)need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined below offset, arbitrary but harmless */

    /* Allocate enough space. */
    ensure_size(n);

    /* Fill base cases if they are missing. */
    if (cache_size == 0) {
        cache[0] = (seq_t)2;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 2) {
        cache[1] = (seq_t)8;
        cache_size = 2;
    }
    if (cache_size == 2 && n >= 3) {
        cache[2] = (seq_t)23;
        cache_size = 3;
    }
    if (cache_size == 3 && n >= 4) {
        cache[3] = (seq_t)53;
        cache_size = 4;
    }

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i is 0‑based index: term a(i+1).  We need a(i-1) and a(i-3). */
        seq_t term = (seq_t)6 * cache[i - 2] - cache[i - 4] + (seq_t)2;
        cache[i] = term;
    }

    if (cache_size < n)
        cache_size = n;

    return cache[n - 1];
}

/*
 * A125922  Sprague-Grundy values for octal game .316.
 * https://oeis.org/A125922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) - a(n-6) + a(n-8) for n > 12 * Initial values
 *            (offset = 1): * a(1)=1, a(2)=2, a(3)=0, a(4)=2, a(5)=1, a(6)=2,
 *            * a(7)=3, a(8)=0, a(9)=1, a(10)=0, a(11)=3, a(12)=0
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation (1‑based) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity (number of elements stored) */
static size_t cache_size = 0;  /* largest index computed so far */

/* base values for 1 … 12 */
static const seq_t base_vals[13] = {
    0,      /* unused index 0 */
    1, 2, 0, 2, 1, 2, 3, 0, 1, 0, 3, 0
};

/* ensure that cache can hold at least indices up to n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;                /* index n must be valid */
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Compute the n‑th term of OEIS A125922 (n ≥ 1)                     */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* not part of the sequence – undefined */

    ensure_capacity(n);

    /* initialise the base region 1 … 12 (if not already done)      */
    if (cache_size < 12) {
        size_t start = cache_size + 1;
        size_t end   = 12;
        if ((idx_t)end > n)
            end = (size_t)n;
        for (size_t i = start; i <= end; ++i)
            cache[i] = base_vals[i];
        cache_size = end;
    }

    /* compute missing terms up to n */
    if ((size_t)n > cache_size) {
        for (idx_t k = (idx_t)(cache_size + 1); k <= n; ++k) {
            if ((idx_t)k <= 12) {                 /* safety copy of base */
                cache[k] = base_vals[(size_t)k];
            } else {
                /* use signed intermediate arithmetic to avoid underflow */
                long long v = (long long)cache[k - 2]
                            - (long long)cache[k - 6]
                            + (long long)cache[k - 8];
                cache[k] = (seq_t)v;
            }
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n];
}

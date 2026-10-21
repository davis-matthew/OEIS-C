/*
 * A209888  Number of binary words of length n containing no subword 01101.
 * https://oeis.org/A209888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for n < 5; for n >= 5: a(n) = 2*(a(n-1)+a(n-4)) –
 *            a(n-3) – a(n-5). Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8,
 *            a(4)=16
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array */
static size_t cache_len = 0;  /* number of entries currently initialised */
static size_t cache_cap = 0;  /* capacity of the allocated array */

/* Ensure the cache can hold all indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n) return;   /* already covers required indices */

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* grow geometrically */

    /* Cast the result of realloc for portability to C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold n. */
    ensure_cache(n);

    /* Compute any missing values up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i < 5) {
            /* 2^i : for arbitrary‑precision seq_t use pow2 to be safe. */
            cache[i] = ((seq_t)1) << i;          /* works for integer types */
            /* If seq_t is a big integer class with operator<< it is defined. */
        } else {
            /* a(i) = 2*(a(i-1)+a(i-4)) – a(i-3) – a(i-5) */
            seq_t temp = cache[i-1] + cache[i-4];   /* a[i-1] + a[i-4] */
            temp <<= 1;                            /* 2 * (... ) */
            temp -= cache[i-3];
            temp -= cache[i-5];
            cache[i] = temp;
        }
    }

    cache_len = (size_t)n + 1;  /* update number of initialised entries */
    return cache[(size_t)n];
}

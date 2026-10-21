/*
 * A231799  Number of (n+1) X (1+1) 0..1 arrays with no element having a
 *          strict majority of its horizontal, vertical, diagonal and
 *          antidiagonal neighbors equal to one.
 * https://oeis.org/A231799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=13, a(3)=32, a(4)=79, a(5)=200, a(6)=500,
 *            a(n)=a(n-1)+2*a(n-2)+5*a(n-3)-2*a(n-5)-4*a(n-6) for n≥7
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i+1)  */
static size_t cache_len = 0;  /* number of terms currently stored */

/* Grow the cache so that it contains terms up to target (1‑based). */
static void ensure_cache(idx_t target)
{
    if (target == 0) return;
    if (cache && (size_t)target <= cache_len) return;

    /* First time: initialise with the fixed 6 terms. */
    if (!cache) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return;          /* allocation failure */
        cache[0] = (seq_t)5;         /* a(1) */
        cache[1] = (seq_t)13;        /* a(2) */
        cache[2] = (seq_t)32;        /* a(3) */
        cache[3] = (seq_t)79;        /* a(4) */
        cache[4] = (seq_t)200;       /* a(5) */
        cache[5] = (seq_t)500;       /* a(6) */
        cache_len = 6;
    }

    /* Extend the array if needed. */
    if ((size_t)target > cache_len) {
        size_t new_len = (size_t)target;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return;          /* allocation failure */
        cache = tmp;
        /* Compute new terms up to target. */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* i is zero‑based; represents a(i+1). */
            seq_t val =
                cache[i - 1]                     /* a(n-1) */
                + cache[i - 2] * (seq_t)2        /* 2*a(n-2) */
                + cache[i - 3] * (seq_t)5        /* 5*a(n-3) */
                - cache[i - 5] * (seq_t)2        /* -2*a(n-5) */
                - cache[i - 6] * (seq_t)4;       /* -4*a(n-6) */
            cache[i] = val;
        }
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0 */
    ensure_cache(n);
    if (cache && (size_t)n <= cache_len)
        return cache[(size_t)n - 1];
    return 0;                     /* allocation failure */
}

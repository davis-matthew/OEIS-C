/*
 * A219100  Unmatched value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          vertical or antidiagonal neighbor in a random 0..2 n X 2 array.
 * https://oeis.org/A219100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 5*a(n-3) - 3*a(n-4) + a(n-5);
 *            Initial values: a(1)=2, a(2)=11, a(3)=42, a(4)=154, a(5)=582;
 *            Recurrence is valid for all n >= 6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1), 0‑based */
static size_t cache_size = 0;    /* number of terms currently cached */

/* Ensure that cache holds terms 1 .. n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                 /* undefined but never requested */

    size_t needed = (size_t)n;
    if (needed < 5) needed = 5;         /* keep first five terms ready */

    if (!cache) {
        cache = (seq_t *)calloc(needed, sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */

        /* Base terms */
        cache[0] = 2;   /* a(1) */
        cache[1] = 11;  /* a(2) */
        cache[2] = 42;  /* a(3) */
        cache[3] = 154; /* a(4) */
        cache[4] = 582; /* a(5) */

        cache_size = 5;
    } else if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_size; i < needed; ++i) {
            /* i corresponds to term a(i+1) */
            seq_t val = cache[i-1] * 4;      /* 4*a(n-1) */
            val -= cache[i-2] * 2;           /* - 2*a(n-2) */
            val += cache[i-3] * 5;           /* + 5*a(n-3) */
            val -= cache[i-4] * 3;           /* - 3*a(n-4) */
            val += cache[i-5];               /* + a(n-5) */
            cache[i] = val;
        }

        cache_size = needed;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe fallback */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}

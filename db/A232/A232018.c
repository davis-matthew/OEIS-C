/*
 * A232018  Number of n X 3 0..2 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 59*a(n-2) + 62*a(n-3) - 275*a(n-4) -
 *            257*a(n-5) + 146*a(n-6) + 84*a(n-7) for n > 8 Initial values:
 *            a(1)=9, a(2)=66, a(3)=852, a(4)=11517, a(5)=156913,
 *            a(6)=2125749, a(7)=28852936, a(8)=391447970
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t cache_len = 0;          /* number of terms currently stored */
static idx_t cache_cap = 0;          /* capacity of the cache array */

/* Ensure that the cache holds at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        /* Initialise with the first 8 terms. */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)9;       /* a(1) */
        if (cache_cap > 1) cache[1] = (seq_t)66;    /* a(2) */
        if (cache_cap > 2) cache[2] = (seq_t)852;   /* a(3) */
        if (cache_cap > 3) cache[3] = (seq_t)11517; /* a(4) */
        if (cache_cap > 4) cache[4] = (seq_t)156913;/* a(5) */
        if (cache_cap > 5) cache[5] = (seq_t)2125749;/* a(6) */
        if (cache_cap > 6) cache[6] = (seq_t)28852936;/* a(7) */
        if (cache_cap > 7) cache[7] = (seq_t)391447970;/* a(8) */
        cache_len = 8;
    }

    if (n <= cache_len) return;

    /* Grow the cache if necessary. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;           /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 8) continue;               /* already initialized */

        /* a(i-1) is cache[i-2], a(i-2) is cache[i-3], ... */
        seq_t a1 = cache[i-2];
        seq_t a2 = cache[i-3];
        seq_t a3 = cache[i-4];
        seq_t a4 = cache[i-5];
        seq_t a5 = cache[i-6];
        seq_t a6 = cache[i-7];
        seq_t a7 = cache[i-8];

        /* Compute positive and negative contributions separately
           to avoid underflow with unsigned types. */
        seq_t pos = ((seq_t)9)  * a1
                  + ((seq_t)59) * a2
                  + ((seq_t)62) * a3
                  + ((seq_t)146)* a6
                  + ((seq_t)84) * a7;

        seq_t neg = ((seq_t)275) * a4
                  + ((seq_t)257) * a5;

        cache[i-1] = pos - neg;           /* result is guaranteed non‑negative */
    }
    cache_len = n;
}

/* Public function as specified by the requirements. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0; return 0 for safety */
    ensure_cache(n);
    return cache[n-1];             /* a(1) is at cache[0] */
}

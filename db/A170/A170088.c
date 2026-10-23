/*
 * A170088  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=31*30^{n-1} for 1≤n≤36,
 *            a(n)=29*a(n-1)+29*a(n-2)+…+29*a(n-36)+435*a(n-37) for n≥37
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* 31 generators, relations (S_i)^2 = (S_i S_j)^37 = I.
 * 32-bit/64-bit or arbitrary-precision seq_t supplied by build. */
static seq_t *cache = NULL;          /* dynamic array of computed values */
static size_t cache_len = 0;         /* number of valid entries in cache */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* Powers of 30 up to 30^36, needed for the first 36 terms. */
static seq_t pow30[37];
static bool pow30_init = false;

/* Initialise geometric powers of 30 once. */
static void init_pow30(void)
{
    if (pow30_init) return;
    pow30[0] = (seq_t)1;
    for (int i = 1; i <= 36; ++i)
        pow30[i] = pow30[i - 1] * ((seq_t)30);
    pow30_init = true;
}

/* Ensure cache has capacity for at least n+1 elements.  */
static void ensure_capacity(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (target <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < target) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure – let program terminate */
    cache   = tmp;
    cache_cap = new_cap;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* Handle small n directly, also initialise powers. */
    if (n < 0) return (seq_t)0;  /* safety, though n is unsigned */
    if (n <= 36) {
        if (n == 0) return (seq_t)1;
        init_pow30();
        return ((seq_t)31) * pow30[(size_t)n - 1];
    }

    /* For n ≥ 37 we use memoised recurrence. */
    ensure_capacity(n);

    /* Initialise first 37 entries if not already done. */
    if (cache_len <= 36) {
        init_pow30();
        size_t start = 0;
        if (cache_len < 37) start = cache_len;
        for (size_t i = start; i <= 36; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else
                cache[i] = ((seq_t)31) * pow30[(size_t)i - 1];
        }
        cache_len = 37;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= 36; ++k)  /* 29 * a(i-k) */
            sum += ((seq_t)29) * cache[i - k];
        sum += ((seq_t)435) * cache[i - 37];  /* 435 * a(i-37) */
        cache[i] = sum;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}

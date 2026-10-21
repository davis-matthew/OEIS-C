/*
 * A238361  Number of length n binary words that contain 111 but do not
 *          contain 000 (as contiguous subwords).
 * https://oeis.org/A238361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238361  
   Number of length‑n binary words that contain 111 but do not contain 000.  
   Computed by the recurrence  
       a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=3, a(5)=8  
       a(n)=2*a(n-1)+a(n-2)-a(n-3)-2*a(n-4)-a(n-5)   (n>5)          */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* memoisation buffer                      */
static size_t cache_cap = 0;      /* current allocation size                  */
static size_t computed_until = 0; /* first index not yet computed            */

/* Ensure that 'cache' can hold at least 'needed' elements.   */
/* Works in both C (malloc/realloc) and C++ (new/delete).      */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;

#ifdef __cplusplus
    /* C++ : allocate with new[], copy old values, deallocate old buffer. */
    seq_t *new_cache = new seq_t[new_cap];
    for (size_t i = 0; i < computed_until; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    /* C : use realloc, which casts to (void*) automatically.        */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main function with static memoisation. */
seq_t a(idx_t n)
{
    /* Base values for 0 <= n < 6 (no dynamic memory required). */
    if (n < 6u) {
        static const seq_t init[6] = {0, 0, 0, 1, 3, 8};
        return init[(size_t)n];
    }

    /* Initialise cache on first use. */
    if (cache == NULL) {
        ensure_cache_capacity(6);
    }

    /* Store the initial values once. */
    if (computed_until == 0) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 3;
        cache[5] = 8;
        computed_until = 6;
    }

    /* Grow the buffer if necessary. */
    ensure_cache_capacity((size_t)n + 1);

    /* Compute any missing terms up to n. */
    if ((size_t)n >= computed_until) {
        for (size_t i = computed_until; i <= (size_t)n; ++i) {
            seq_t temp = cache[i - 1];
            temp = temp + temp;                 /* 2*a(i-1) */
            temp = temp + cache[i - 2];        /* + a(i-2) */
            temp = temp - cache[i - 3];        /* - a(i-3) */
            seq_t two_ai4 = cache[i - 4];
            two_ai4 = two_ai4 + two_ai4;      /* 2*a(i-4) */
            temp = temp - two_ai4;             /* - 2*a(i-4) */
            temp = temp - cache[i - 5];        /* - a(i-5) */
            cache[i] = temp;
        }
        computed_until = (size_t)n + 1;
    }

    return cache[(size_t)n];
}

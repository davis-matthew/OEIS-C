/*
 * A239075  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 3: |p(i)-i| <= 3.
 * https://oeis.org/A239075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-4)-a(n-7) (n>=7) Initial values:
 *            a0=1,a1=1,a2=2,a3=4,a4=10,a5=22,a6=48
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>   /* for size_t and SIZE_MAX */

/* Static cache of previously computed terms */
static seq_t *cache = NULL;          /* dynamically allocated array of seq_t */
static size_t cache_size = 0;        /* number of allocated slots */
static size_t computed_max = SIZE_MAX;   /* highest index that is known */

/* Ensure that cache has at least (n+1) slots */
static void ensure_capacity(idx_t n)
{
    /* n is idx_t (uint64_t); cast to size_t for indexing */
    size_t need = (size_t)n + 1;
    if (cache_size >= need) return;

    size_t new_cap = cache_size ? cache_size * 2 : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the unlikely event realloc fails tmp will be NULL; let it propagate */
    cache = tmp;
    cache_size = new_cap;
}

/* Initialise cache with the first 7 terms of the sequence */
static void init_cache(void)
{
    ensure_capacity(6);                 /* indices 0 .. 6 inclusive */
    static const seq_t init[7] = {
        (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)4, (seq_t)10, (seq_t)22, (seq_t)48
    };
    memcpy(cache, init, sizeof(init));
    computed_max = 6;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        init_cache();
    }

    /* If the requested term is already known, return it */
    if ((size_t)n <= computed_max) {
        return cache[(size_t)n];
    }

    /* Ensure the array is large enough */
    ensure_capacity(n);

    /* Compute terms from computed_max+1 up to n */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        /* Use the recurrence
           a[i] = 2*a[i-1] + 2*a[i-4] - a[i-7]   for all i >= 7
           For i < 7, values have already been initialised.
        */
        seq_t val = cache[i-1] + cache[i-1];      /* 2*a[i-1]   */
        val = val + cache[i-4] + cache[i-4];      /* +2*a[i-4]  */
        if (i >= 7) {
            val = val - cache[i-7];               /* -a[i-7]    */
        }
        cache[i] = val;
    }

    computed_max = (size_t)n;
    return cache[(size_t)n];
}

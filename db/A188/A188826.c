/*
 * A188826  Number of 4 X n binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) - 15*a(n-3) + 3*a(n-4) + 5*a(n-5) -
 *            a(n-6) (n > 7). Initial values: a(1)=16, a(2)=25, a(3)=81,
 *            a(4)=256, a(5)=841, a(6)=2704, a(7)=8836
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper function to ensure the cache can hold at least (n+1) elements. */
static void ensure_capacity(idx_t n)
{
    static size_t capacity = 0;
    if (n + 1 <= capacity)
        return;

    size_t newcap = capacity ? capacity << 1 : 16;
    while (newcap <= (size_t)n)
        newcap <<= 1;

    /* Allocate or reallocate the cache.  The cast is legal for both C and C++. */
    static seq_t *cache = NULL;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    capacity = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static state for memoization */
    static idx_t cached = 0;
    static seq_t *cache = NULL;
    static size_t capacity = 0;

    /* Allocate or grow the cache as needed */
    if (n + 1 > capacity) {
        size_t newcap = capacity ? capacity << 1 : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* If already computed, return cached value */
    if (n <= cached)
        return cache[n];

    /* Compute terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* Handle initial values explicitly */
        if (i == 1) {
            cache[1] = (seq_t)16;
        } else if (i == 2) {
            cache[2] = (seq_t)25;
        } else if (i == 3) {
            cache[3] = (seq_t)81;
        } else if (i == 4) {
            cache[4] = (seq_t)256;
        } else if (i == 5) {
            cache[5] = (seq_t)841;
        } else if (i == 6) {
            cache[6] = (seq_t)2704;
        } else if (i == 7) {
            cache[7] = (seq_t)8836;
        } else {
            /* Recurrence: a(i) = 3*a(i-1) + 5*a(i-2) - 15*a(i-3) + 3*a(i-4) + 5*a(i-5) - a(i-6) */
            seq_t total = ((seq_t)3 * cache[i-1]) + ((seq_t)5 * cache[i-2]) +
                          ((seq_t)3 * cache[i-4]) + ((seq_t)5 * cache[i-5]);
            seq_t sub   = ((seq_t)15 * cache[i-3]) + cache[i-6];
            cache[i] = total - sub;
        }
    }

    cached = n;
    return cache[n];
}

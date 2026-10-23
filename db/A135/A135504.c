/*
 * A135504  a(1)=1; for n>1, a(n) = a(n-1) + lcm(a(n-1),n).
 * https://oeis.org/A135504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(1)=1; for n>1, a(n) = a(n-1) + lcm(a(n-1), n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* On first call allocate cache with element for n=1.
       On subsequent calls extend cache only as far as needed.          */
    static seq_t *cache = NULL;
    static idx_t computed = 0; /* number of terms already in cache */

    if (n == 0)   /* undefined, but handle gracefully */
        return (seq_t)0;

    /* Allocate enough space for the requested term if not yet allocated. */
    if (cache == NULL) {
        /* allocate at least n elements; allocate exactly n for simplicity    */
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        /* first term a(1) is defined as 1 */
        cache[0] = (seq_t)1;
        computed = 1;
    } else if (n > computed) {
        /* extend the array to hold up to term n */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
    }

    /* If the requested term is already computed, return it. */
    if (n <= computed)
        return cache[n - 1];

    /* Compute missing terms sequentially.                          */
    for (idx_t i = computed; i < n; ++i) {
        /* i is 0‑based index; current term number is i+1         */
        seq_t prev = cache[i - 1];                 /* a(i)          */
        seq_t g    = gcd(prev, (seq_t)(i + 1));    /* gcd(a(i), i+1) */
        seq_t lcm  = (prev / g) * (seq_t)(i + 1);   /* lcm(a(i), i+1) */
        cache[i]   = prev + lcm;                   /* a(i+1)        */
    }
    computed = n;
    return cache[n - 1];
}

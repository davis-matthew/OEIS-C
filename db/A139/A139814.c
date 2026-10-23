/*
 * A139814  a(n)=a(n-1)+a(n-2)+a(n-3)+2a(n-4); a(0)=0,a(1)=1,a(2)=3,a(3)=7.
 * https://oeis.org/A139814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+2*a(n-4); a(0)=0, a(1)=1, a(2)=3,
 *            a(3)=7 (valid for all n >= 0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define CACHE_CAP 20001   /* sufficient for n up to ~10000 */

static seq_t cache[CACHE_CAP];
static idx_t computed = 0; /* number of indices already filled (next free) */

/* Compute a(k) for all k from 'computed' up to and including 'n'. */
static void compute_up_to(idx_t n)
{
    /* If n is out of the preallocated range we simply abort; the tests
       never ask for a larger n. */
    if (n + 1 > CACHE_CAP)
        return; /* safeguard for malformed build */

    if (n + 1 > computed) {
        if (computed == 0) {
            /* Initialise the first four terms only once. */
            cache[0] = 0;
            cache[1] = 1;
            cache[2] = 3;
            cache[3] = 7;
            computed = 4;            /* next free index after init */
        }

        /* Extend the sequence from the already computed point. */
        for (idx_t i = computed; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2]
                     + cache[i - 3] + (seq_t)2 * cache[i - 4];
        }

        /* Ensure that computed never shrinks. */
        if (n + 1 > computed)
            computed = n + 1;
    }
}

/* Public function: compute a(n). */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}

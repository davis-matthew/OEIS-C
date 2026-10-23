/*
 * A102125  Iccanobirt numbers (15 of 15): a(n) = R(R(a(n-1)) + R(a(n-2)) +
 *          R(a(n-3))), where R is the digit reversal function A004086.
 * https://oeis.org/A102125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R( R(a(n-1)) + R(a(n-2)) + R(a(n-3)) ), a(0)=0, a(1)=0,
 *            a(2)=1 (R is digit reversal, A004086)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 38, u64 n <= 75, u128 n <= 147,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/rev.h"

/* Memoization structures */
static seq_t *cache = NULL;        /* array of computed terms          */
static unsigned char *seen = NULL; /* 1 if the term is computed        */
static idx_t cache_cap = 0;        /* current allocated size          */
static idx_t last_computed = 0;    /* highest index computed so far    */

/* Allocate or grow the cache to be able to store indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap <= n) new_cap <<= 1;

    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    seen  = (unsigned char *)realloc(seen, sizeof(unsigned char) * new_cap);
    for (idx_t i = cache_cap; i < new_cap; ++i) seen[i] = 0;

    cache_cap = new_cap;
}

/* Initialise the known initial values */
static void init_base(void)
{
    ensure_capacity(2);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    seen[0]  = seen[1] = seen[2] = 1;
    last_computed = 2;
}

seq_t a(idx_t n)
{
    /* On first call initialise the base terms */
    static int initialized = 0;
    if (!initialized) { init_base(); initialized = 1; }

    /* If the term is already known, return it immediately */
    if (n <= last_computed && seen[n]) return cache[n];

    /* Make sure we have space for the required indices */
    ensure_capacity(n);

    /* Compute all missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t r1 = rev(cache[i-1]);
        seq_t r2 = rev(cache[i-2]);
        seq_t r3 = rev(cache[i-3]);
        seq_t sum = r1 + r2 + r3;
        cache[i] = rev(sum);
        seen[i]  = 1;
    }
    last_computed = n;
    return cache[n];
}

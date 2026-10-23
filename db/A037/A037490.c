/*
 * A037490  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1.
 * https://oeis.org/A037490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2, and for n ≥ 2 a(n) = 5*a(n-1) + (n is odd ? 2 : 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition of OEIS A037490. */

#include <stdlib.h>
/* seq_t and idx_t are defined by the build environment. */

static seq_t  *cache     = NULL;   /* 1‑based array of computed terms   */
static idx_t  last_used  = 0;      /* highest index that has been computed */
static size_t capacity  = 0;       /* size of allocated array (first element unused) */

/* Ensure the cache has room for indices up to n (inclusive).  */
static void ensure_capacity(idx_t n)
{
    if (capacity > n)
        return;

    /* Start with a small size if no cache yet. */
    if (capacity == 0) {
        capacity = 2;
        cache    = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }

    /* Double until we exceed n. */
    while (capacity <= n) {
        capacity *= 2;
    }
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
}

/* Public function that returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined for offset 0; return 0 by convention. */

    if (n <= last_used)
        return cache[n];

    ensure_capacity(n);

    /* Initialize the first term if not already done. */
    if (last_used == 0) {
        cache[1] = (seq_t)2;
        last_used = 1;
    }

    /* Compute successive terms up to n. */
    for (idx_t i = last_used + 1; i <= n; ++i) {
        /* d_i = 2 if i odd, else 1 */
        seq_t digit = (i & 1) ? (seq_t)2 : (seq_t)1;
        cache[i] = cache[i-1] * (seq_t)5 + digit;
    }
    last_used = n;
    return cache[n];
}

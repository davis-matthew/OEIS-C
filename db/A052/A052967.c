/*
 * A052967  Expansion of (1 - x)/(1 - 2*x - x^2 + x^4).
 * https://oeis.org/A052967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052967: expansion of (1 - x)/(1 - 2*x - x^2 + x^4)
   Recurrence: a(n+4) = a(n+2) + 2*a(n+3) - a(n)   (n ≥ 0)
   Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=7 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached terms of the sequence */
static idx_t   cap   = 0;     /* allocated capacity of cache */
static idx_t   len   = 0;     /* number of terms currently stored */

static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;
    idx_t new_cap = cap ? cap << 1 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) /* allocation failure */
        exit(1);
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* First call: initialize the base terms and allocate a small cache */
    if (len == 0) {
        ensure_capacity(3);      /* need space for indices 0..3 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        len = 4;                 /* indices 0..3 are ready */
    }

    /* Extend the cache up to n if necessary */
    if (n >= len) {
        ensure_capacity(n);
        for (idx_t i = len; i <= n; ++i) {
            /* a(i) = a(i-2) + 2*a(i-1) - a(i-4)  (i ≥ 4) */
            seq_t term = cache[i-2] + (seq_t)2 * cache[i-1] - cache[i-4];
            cache[i] = term;
        }
        len = n + 1;
    }

    return cache[n];
}

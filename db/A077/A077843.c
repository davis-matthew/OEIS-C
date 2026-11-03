/*
 * A077843  Expansion of (1-x)/(1-2*x-2*x^2-2*x^3).
 * https://oeis.org/A077843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=4, a(n)=2*a(n-1)+2*a(n-2)+2*a(n-3) for n>=3
 *            (coefficients of (1‑x)/(1‑2x‑2x²‑2x³) )
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 2120 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;   /* memoised values */
static idx_t capacity = 0;    /* allocated array size */
static idx_t size = 0;        /* number of values stored */

/* ensure that cache can hold terms 0..n */
static void ensure_capacity(idx_t n)
{
    if (n < size) return;                 /* already computed */

    if (capacity == 0) {
        capacity = 4;
        cache = mem_new(seq_t, capacity);
    }

    while (size <= n) {
        if (size >= capacity) {
            capacity <<= 1;              /* grow */
            cache = mem_resize(seq_t, cache, capacity);
        }

        seq_t term;
        if (size == 0) {                 /* a(0) */
            term = 1;
        } else if (size == 1) {          /* a(1) */
            term = 1;
        } else if (size == 2) {          /* a(2) */
            term = 4;
        } else {                         /* recurrence for n>=3 */
            term = cache[size-1]*2 + cache[size-2]*2 + cache[size-3]*2;
        }
        cache[size++] = term;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return cache[n];
}

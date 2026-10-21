/*
 * A222390  Nonnegative integers m such that 10*m*(m+1)+1 is a square.
 * https://oeis.org/A222390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A222390: Nonnegative integers m such that 10*m*(m+1)+1 is a square.
   The sequence satisfies the 5‑term linear recurrence
     a(n) = a(n-1) + 38*a(n-2) - 38*a(n-3) - a(n-4) + a(n-5)
   with initial values
     a(1)=0, a(2)=3, a(3)=15, a(4)=132, a(5)=588.
   The function memoises already computed terms so that calls with any
   ordering of n are handled efficiently. */
#include <stdlib.h>

static seq_t *cache = NULL;       /* cache[0] holds a(1) */
static idx_t cache_size = 0;      /* number of terms stored */

/* Extend the cache so that it contains at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Grow the array to size n. */
    seq_t *tmp = (seq_t *)realloc(cache,
                                  (size_t)n * sizeof(seq_t));
    if (!tmp) {
        abort();          /* not enough memory; terminate safely */
    }
    cache = tmp;

    /* Compute any new terms, using the recurrence. */
    for (idx_t i = cache_size; i < n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;          /* a(1) */
        } else if (i == 1) {
            cache[1] = (seq_t)3;          /* a(2) */
        } else if (i == 2) {
            cache[2] = (seq_t)15;         /* a(3) */
        } else if (i == 3) {
            cache[3] = (seq_t)132;        /* a(4) */
        } else if (i == 4) {
            cache[4] = (seq_t)588;        /* a(5) */
        } else {
            /* Recurrence: a(n) = a(n-1)+38*a(n-2)-38*a(n-3)-a(n-4)+a(n-5) */
            cache[i] = cache[i-1]
                     + (cache[i-2] * (seq_t)38)
                     - (cache[i-3] * (seq_t)38)
                     - cache[i-4]
                     + cache[i-5];
        }
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; return zero as a safe fallback */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n-1];   /* cache is 0‑based, OEIS is 1‑based */
}

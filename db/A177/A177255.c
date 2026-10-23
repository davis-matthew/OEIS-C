/*
 * A177255  a(n) = Sum_{j=1..n} j*B(j-1), where B(k) = A000110(k) are the Bell
 *          numbers.
 * https://oeis.org/A177255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} j * B(j-1), where B(k) = Bell(k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of Bell numbers B[0..] and a[n] */
static seq_t *B_cache = NULL;   /* B[0..]            */
static seq_t *A_cache = NULL;   /* a[0..]            */
static idx_t  cache_cap = 0;    /* number of stored entries */

/* Return the nth term of OEIS A177255.  Memoises all previous values. */
seq_t a(idx_t n)
{
    if (n < cache_cap) /* already computed */
        return A_cache[n];

    /* Grow cache to include n */
    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;
    B_cache = (seq_t*)realloc(B_cache, new_cap * sizeof(seq_t));
    A_cache = (seq_t*)realloc(A_cache, new_cap * sizeof(seq_t));

    if (old_cap == 0) {        /* initialise base cases */
        B_cache[0] = (seq_t)1; /* B(0) = 1          */
        A_cache[0] = (seq_t)0; /* a(0) = 0          */
        old_cap    = 1;
    }

    /* Compute all new entries up to n */
    for (idx_t i = old_cap; i <= n; ++i) {
        /* 1. Update a(i) using the previous result and B(i-1) */
        seq_t prod = B_cache[i - 1];
        prod *= (seq_t)i;                 /* i * B(i-1)        */
        A_cache[i] = A_cache[i - 1] + prod;  /* a(i) = a(i-1)+prod */

        /* 2. Compute B(i) via the recurrence
           B(i) = Σ_{k=0}^{i-1} C(i-1,k) * B(k)          */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            if (k > 0) { /* update binom to C(i-1,k) from C(i-1,k-1) */
                binom *= (seq_t)(i - k);   /* multiply by (i-1-(k-1)) = i-k */
                binom /= (seq_t)k;         /* exact division */
            }
            sum += binom * B_cache[k];
        }
        B_cache[i] = sum;
    }

    cache_cap = new_cap;
    return A_cache[n];
}

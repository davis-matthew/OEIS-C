/*
 * A023547  Convolution of natural numbers >= 2 and A023532.
 * https://oeis.org/A023547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the convolution of the sequence of natural numbers >= 2
 *            with OEIS A023532. The sequence satisfies the recurrence a(1) =
 *            2 a(n+1) = a(n) + 2*b(n) + Σ_{j=0}^{n-1} b(j) for n ≥ 1 where
 *            b(k) = 0 iff k = (m^2 + 3m – 2)/2 for some integer m ≥ 1,
 *            otherwise b(k) = 1. The values b(k) are dense 1‑v...
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(v)) for unsigned 64–bit values. */
static inline idx_t int_sqrt(idx_t v)
{
    idx_t x = 0;
    while ((x + 1) * (x + 1) <= v) {
        ++x;
    }
    return x;
}

/* Test whether index k is one of the zero positions of b(k).  These are
   exactly the values k = (m^2 + 3m – 2)/2 for m ≥ 1. */
static inline bool zero_index(idx_t k)
{
    idx_t val = 8 * k + 17;          /* 2k^2 + 6k + 2 = 8k + 17?  */
    idx_t t   = int_sqrt(val);
    if (t * t != val) return false; /* not a perfect square */
    /* Now t = sqrt(8k + 17).  k is a zero index iff (t - 3) is even
       and >= 0.  This comes from solving k^2 + 3k - (2k+2) = 0. */
    if (t < 3) return false;
    if ( ((t - 3) & 1) != 0 ) return false;
    return true;
}

/* Global cache of computed a(n).  We grow the cache only when needed. */
static seq_t *a_cache   = NULL;  /* array storing a(1)..a(computed_max) */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t  computed_max = 0;   /* number of a-values currently stored */
static seq_t  sum_b    = 0;       /* Σ_{j=0}^{k-1} b(j) for the next k */

/* Return the nth term of the sequence (OEIS A023547). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */

    /* Ensure the cache holds at least n terms. */
    if (n > computed_max) {
        /* Resize cache if necessary. */
        if (n > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_arr) exit(EXIT_FAILURE);
            for (size_t i = 0; i < cache_cap; ++i)
                new_arr[i] = a_cache[i];
            free(a_cache);
            a_cache = new_arr;
            cache_cap = new_cap;
        }

        /* Initialize first term if the cache is empty. */
        if (computed_max == 0) {
            a_cache[0] = (seq_t)2;  /* a(1) */
            sum_b      = (seq_t)1;  /* b(0) */
            computed_max = 1;
        }

        /* Compute a-values up to n. */
        for (idx_t k = computed_max; k < n; ++k) {
            seq_t b_k = zero_index(k) ? (seq_t)0 : (seq_t)1;
            seq_t diff = (seq_t)2 * b_k + sum_b;
            a_cache[k] = a_cache[k - 1] + diff;
            sum_b += b_k;
        }
        computed_max = n;
    }

    return a_cache[n - 1];
}

/*
 * A018825  Numbers that are not the sum of 2 nonzero squares.
 * https://oeis.org/A018825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n), the n‑th positive integer that cannot be
   expressed as a sum of two nonzero squares (i.e., a² + b² with a,b>0). */
/* It uses a simple incremental search with caching to be able to answer
   arbitrary queries for any n >= 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached terms of the sequence */
static seq_t *cache   = NULL;   /* a(k) values, 0‑based index */
static idx_t  cap     = 0;      /* current allocation size */
static idx_t  count   = 0;      /* number of values stored */
static idx_t  next    = 1;      /* next integer to test */

/* Integer square root for idx_t using binary search. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Return true iff x can be written as a² + b² with a,b > 0. */
static bool is_sum_of_two_nonzero_squares(idx_t x)
{
    /* 1 cannot be represented; 2 = 1² + 1² is representable. */
    if (x == 1) return false;
    if (x == 2) return true;

    idx_t max_a = isqrt_u64(x - 1);  /* a^2 < x */
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t b2 = x - a * a;
        if (b2 == 0) continue;      /* would give b = 0 */
        idx_t b = isqrt_u64(b2);
        if ((unsigned __int128)b * b == b2) return true;
    }
    return false;
}

/* Ensure that cache contains at least n terms (1‑based n). */
static void ensure_n_terms(idx_t n)
{
    if (count >= n) return;

    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (cap < n) {
        while (cap < n) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    while (count < n) {
        idx_t m = next++;                     /* candidate integer */
        if (!is_sum_of_two_nonzero_squares(m))
            cache[count++] = (seq_t)m;        /* store the term */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n=0 */
    ensure_n_terms(n);
    return cache[n - 1];
}

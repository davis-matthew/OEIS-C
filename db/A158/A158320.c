/*
 * A158320  a(1)=0, a(n) is the smallest nonprime > (n-1)^smallest digit of
 *          a(n-1).
 * https://oeis.org/A158320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_leq_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A158320
   a(1) = 0
   a(n)  = the smallest composite (non‑prime) integer > (n‑1)^d,
   where d is the smallest decimal digit of a(n‑1). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_leq_seq.h"

/* Helpers */

/* Return the smallest decimal digit of x (x ≥ 0). */
static int min_digit(seq_t x)
{
    if (x == (seq_t)0) return 0;
    int best = 9;
    seq_t t = x;
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)10);
        if (d < best) {
            best = d;
            if (best == 0) break;
        }
        t /= (seq_t)10;
    }
    return best;
}

/* Static cache of computed terms.  cache[i] holds a(i+1) (1‑based). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* number of terms stored */

/* Compute a(n) for any n ≥ 1, supporting arbitrary access order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but return 0 */

    /* Ensure the cache is large enough. */
    if (cache_len == 0) {
        /* Allocate initial capacity. */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;      /* a(1) */
        cache_len = 1;
    }
    if (cache_len < n) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t k = cache_len; k < n; ++k) { /* k is 1‑based index of term to compute minus 1 */
            /* a(k+1) uses a(k) which is at cache[k-1] */
            seq_t prev = cache[k-1];
            int d = min_digit(prev);           /* smallest digit of a(k) */
            seq_t base = (seq_t)k;             /* n-1 = k */
            seq_t power = (seq_t)1;
            for (int e = 0; e < d; ++e)        /* compute base^d */
                power *= base;
            seq_t cand = power + (seq_t)1;     /* start just above base^d */
            while (is_prime_leq_seq(cand))
                ++cand;
            cache[k] = cand;                   /* store a(k+1) */
        }
        cache_len = n;
    }
    return cache[n-1];
}

/*
 * A073375  Fifth convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} C(n-k+5,5) · C(n-k,k) · 2^k (OEIS A073375)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 46, u128 n <= 104,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

static seq_t *cache   = NULL;   /* cached values of a(n)               */
static uint8_t *valid = NULL;   /* 1 if cache[n] holds a valid value   */
static size_t   cap    = 0;     /* allocated length of cache            */

/* Ensure cache can hold values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n < (idx_t)cap)
        return;
    size_t new_cap = (size_t)(n + 128) * 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    valid = (uint8_t *)realloc(valid, new_cap * sizeof(uint8_t));
    if (!cache || !valid)
        exit(EXIT_FAILURE);  /* memory allocation failed */
    for (size_t i = cap; i < new_cap; ++i)
        valid[i] = 0;
    cap = new_cap;
}

/* Compute the n-th term of sequence A073375 */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    if (valid[n])
        return cache[n];

    seq_t sum   = 0;
    seq_t pow2  = 1;                 /* 2^k, updated each iteration */
    idx_t limit = n / 2;             /* floor(n/2) */

    for (idx_t k = 0; k <= limit; ++k) {
        idx_t n1 = n - k;
        seq_t term1 = binomial((idx_t)(n1 + 5), 5);  /* C(n-k+5,5) */
        seq_t term2 = binomial(n1, k);               /* C(n-k,k)   */
        seq_t prod  = term1 * term2;
        prod *= pow2;
        sum += prod;
        pow2 <<= 1;                          /* multiply by 2 for next k */
    }

    cache[n] = sum;
    valid[n] = 1;
    return sum;
}

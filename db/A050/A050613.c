/*
 * A050613  Products of distinct terms of 1 and rest from A001566: a(n) =
 *          Product_{i=0..floor(log_2(n+1))} L(2^i)^bit(n,i).
 * https://oeis.org/A050613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..⌊log₂(n+1)⌋} L(2^i)^{bit(n,i)}
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* L(k) is the Lucas sequence: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2). */

#include <stdint.h>
#include <stdlib.h>

static inline idx_t floor_log2_idx(idx_t v)
{
    /* v > 0 */
#if defined(__GNUC__) || defined(__clang__)
    return (idx_t)(63 - __builtin_clzll(v));
#else
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* Cached Lucas numbers up to the largest required index */
static seq_t *lucas_cache = NULL;
static idx_t lucas_cache_size = 0;

seq_t a(idx_t n)
{
    /* Determine the maximum index of Lucas number needed (2^i) */
    idx_t max_bit = 0;
    if (n != (idx_t)-1) {            /* avoid overflow when n is max */
        max_bit = floor_log2_idx(n + 1);
    }
    idx_t max_pow = (idx_t)1 << max_bit;   /* 2^max_bit */

    /* Expand the cache if necessary */
    if (lucas_cache_size <= max_pow) {
        idx_t new_size = max_pow + 1;
        if (lucas_cache == NULL) {
            lucas_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            lucas_cache = (seq_t*)realloc(lucas_cache, new_size * sizeof(seq_t));
        }
        /* Initialize newly added entries */
        if (lucas_cache_size == 0) {
            lucas_cache[0] = (seq_t)2;
            lucas_cache[1] = (seq_t)1;
            lucas_cache_size = 2;
        }
        /* Compute Lucas numbers up to the new size */
        for (idx_t i = lucas_cache_size; i < new_size; ++i) {
            lucas_cache[i] = lucas_cache[i - 1] + lucas_cache[i - 2];
        }
        lucas_cache_size = new_size;
    }

    /* Compute the product according to the binary expansion of n */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i <= max_bit; ++i) {
        if ((n >> i) & 1ull) {
            idx_t idx = (idx_t)1 << i;
            result *= lucas_cache[idx];
        }
    }
    return result;
}

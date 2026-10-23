/*
 * A049922  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3 and a(3) = 2.
 * https://oeis.org/A049922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=2. For n ≥ 4: a(n) = (a(1)+⋯+a(n‑1)) –
 *            a(m), where m = n‑1 – 2^p, and p is the unique integer with 2^p
 *            < n‑1 ≤ 2^(p+1) (equivalently p = ⌊log₂(n‑2)⌋)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cache of already computed values and their prefix sums. */
static seq_t *values = NULL;   /* values[1] … values[m]   */
static seq_t *prefix = NULL;   /* prefix[i] = sum_{k=1..i} values[k] */
static idx_t cache_size = 0;   /* number of entries currently cached  */
static idx_t cache_cap  = 0;   /* allocated capacity (≥ cache_size)      */

/* Grow the cache to at least new_cap elements. */
static void ensure_cache_size(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* Double capacity each time until it is large enough. */
    if (cache_cap == 0)
        cache_cap = 8;
    while (cache_cap < new_cap)
        cache_cap *= 2;

    /* Reallocate.  We keep index 0 unused, so allocate +1. */
    values  = (seq_t *)realloc(values,  (cache_cap + 1) * sizeof(seq_t));
    prefix  = (seq_t *)realloc(prefix,  (cache_cap + 1) * sizeof(seq_t));
}

/* Main routine. */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined but guard against underflow */
        return (seq_t)0;

    /* Extend cache if necessary. */
    if (n > cache_size) {
        ensure_cache_size(n);   /* allocate enough space */

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
                prefix[1] = val;
            } else if (i == 2) {
                val = (seq_t)3;
                prefix[2] = prefix[1] + val;
            } else if (i == 3) {
                val = (seq_t)2;
                prefix[3] = prefix[2] + val;
            } else {
                /* Compute m = i-1 - 2^p, with p = floor(log2(i-2)) */
                idx_t x   = i - 2;                                 /* i-2 ≥ 1 for i ≥ 4 */
                idx_t p   = 63 - __builtin_clzll(x);               /* floor(log2(x)) */
                idx_t pow = (idx_t)1 << p;
                idx_t m   = i - 1 - pow;                           /* m ≥ 1 */

                /* a(i) = prefix[i-1] - values[m] */
                val = prefix[i-1] - values[m];
                prefix[i] = prefix[i-1] + val;
            }
            values[i] = val;
        }
        cache_size = n;
    }

    return values[n];
}

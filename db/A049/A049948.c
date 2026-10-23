/*
 * A049948  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A049948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+a(2)+...+a(n-1)+a(m) (n ≥ 4) where m = 2*n - 3 -
 *            2^(p+1) and p is the unique integer with 2^p < n-1 ≤ 2^(p+1).
 *            Initial values: a(1)=1, a(2)=2, a(3)=1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache   = NULL; /* a[1]..a[max]   */
static seq_t *cum_cache = NULL; /* cum[0]=0; cum[i]=Σ_{k=1}^{i} a[k] */
static idx_t  cache_size   = 0;
static idx_t  computed_up_to = 0;

/* Ensure the cache arrays can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 8;
    while (new_size < n) new_size <<= 1;

    a_cache    = (seq_t *)realloc(a_cache,    (new_size + 1) * sizeof(seq_t));
    cum_cache  = (seq_t *)realloc(cum_cache,  (new_size + 1) * sizeof(seq_t));

    if (cache_size == 0) {
        cum_cache[0] = (seq_t)0;          /* sum of zero terms */
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;     /* undefined, but safe */

    ensure_capacity(n);

    /* Compute any missing terms up to n in increasing order */
    while (computed_up_to < n) {
        ++computed_up_to;

        if (computed_up_to == 1) {
            a_cache[1]     = (seq_t)1;
            cum_cache[1]   = a_cache[1];
        } else if (computed_up_to == 2) {
            a_cache[2]     = (seq_t)2;
            cum_cache[2]   = cum_cache[1] + a_cache[2];
        } else if (computed_up_to == 3) {
            a_cache[3]     = (seq_t)1;
            cum_cache[3]   = cum_cache[2] + a_cache[3];
        } else {
            idx_t ks = computed_up_to - 1;   /* n-1 */

            /* floor(log2(ks)) */
            idx_t lo = 0;
            idx_t tmp = ks;
            while (tmp >>= 1) ++lo;

            /* Determine p: if ks is a power of two, p = lo-1 else p = lo */
            idx_t p = (( ((idx_t)1) << lo) == ks ) ? (lo - 1) : lo;

            idx_t pow_p1 = ((idx_t)1) << (p + 1);
            idx_t m = 2 * computed_up_to - 3 - pow_p1;

            /* m should already be computed (m <= computed_up_to-1) */
            seq_t a_m = a_cache[m];
            seq_t sum_before = cum_cache[computed_up_to - 1];

            a_cache[computed_up_to] = sum_before + a_m;
            cum_cache[computed_up_to] = sum_before + a_cache[computed_up_to];
        }
    }

    return a_cache[n];
}

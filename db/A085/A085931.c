/*
 * A085931  Leading diagonal of A085930.
 * https://oeis.org/A085931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=2; for n>1 let k=floor((-3+sqrt(9+8(n-1)))/2). Let n_spike
 *            = k(k+3)/2+1. Then if n==n_spike: a(n)=a(n-1)+k²+5k+2 else :
 *            a(n)=a(n-1)+n+k. In other words a(n)=a(n-1)+d(n) with
 *            d(n)=k²+5k+2 if n==k(k+3)/2+1 d(n)=n+k otherwise
 * Range:     u8 n <= 17, u16 n <= 337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* simple cache, grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* next free index (size of valid part) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* allocate cache up to size n+1 (index 0 unused) on first call */
    if (cache_len == 0) {
        cache_cap = 128;               /* initial capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = 0;                  /* dummy for index 0 */
        cache[1] = 2;                  /* a(1) */
        cache_len = 2;                 /* we have values up to index 1 */
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return 0;   /* allocation failed – unlikely in tests */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* compute a(i) up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        idx_t k;
        if (i > 1) {
            /* compute k = floor((-3+sqrt(9+8(i-1)))/2) */
            double arg = 9.0 + 8.0 * (double)(i - 1);
            /* add tiny epsilon to guard against floating point rounding */
            idx_t s = (idx_t)(sqrt(arg) + 1e-9);
            k = (s > 3) ? (s - 3) / 2 : 0;
        } else {
            k = 0;
        }

        /* n_spike = k(k+3)/2 + 1, using seq_t to avoid overflow */
        seq_t n_spike = (seq_t)k * (k + 3) / 2 + 1;
        seq_t diff;

        if ((seq_t)i == n_spike) {
            seq_t k_seq = (seq_t)k;
            diff = k_seq * k_seq + 5 * k_seq + 2;  /* spike increment */
        } else {
            diff = (seq_t)i + k;                     /* regular increment */
        }

        cache[i] = cache[i - 1] + diff;
    }

    cache_len = n + 1;   /* we have computed up to index n */
    return cache[n];
}

/*
 * A239059  Sum of the two smallest parts from the partitions of 4n into 4
 *          parts with smallest part = 1.
 * https://oeis.org/A239059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8) Initial values: a(1)=2, a(2)=9,
 *            a(3)=27, a(4)=61, a(5)=108, a(6)=178, a(7)=276, a(8)=395.
 *            Recurrence is applied for n > 8. The function memoises results
 *            for efficient out‑of‑order calls
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 1934, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static const seq_t init_vals[8] = {
    (seq_t)2, (seq_t)9, (seq_t)27, (seq_t)61,
    (seq_t)108, (seq_t)178, (seq_t)276, (seq_t)395
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t needed = (size_t)n;
    if (needed > cache_len) {
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = cache_len; i < needed; ++i) {
            size_t idx = i + 1; /* 1‑based index */
            seq_t val;
            if (idx <= 8) {
                val = init_vals[idx - 1];
            } else {
                /* recurrence: a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8) */
                seq_t p = ((seq_t)2) * cache[idx - 2] +
                          ((seq_t)2) * cache[idx - 4] +
                          ((seq_t)2) * cache[idx - 6] +
                          ((seq_t)2) * cache[idx - 8];
                seq_t s =      cache[idx - 3] +
                          ((seq_t)4) * cache[idx - 5] +
                          cache[idx - 7] +
                          cache[idx - 9];
                val = p - s;
            }
            cache[i] = val;
        }
        cache_len = needed;
    }
    return cache[n - 1];
}

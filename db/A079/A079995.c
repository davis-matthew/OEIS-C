/*
 * A079995  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={2}.
 * https://oeis.org/A079995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +2*a(n-2)+3*a(n-3)+4*a(n-4)+6*a(n-5)+13*a(n-6)
 *            -9*a(n-7)-11*a(n-8)-7*a(n-9)+5*a(n-10)-a(n-11)-11*a(n-12)+3*a(n-13)
 *            +9*a(n-14)+4*a(n-15)-4*a(n-16)-a(n-17)+2*a(n-18)-a(n-19)-a(n-20),
 *            a(0)=1,a(1)=1,a(2)=2,a(3)=4,a(4)=14,a(5)=39,a(6)=103,a(7)=255,a(8)=665,
 *            a(9)=1741,a(10)=4605,a(11)=120...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int coeffs[20] = {
    1,  2,  3,  4,  6, 13, -9, -11, -7,  5,
   -1, -11,  3,  9,  4, -4, -1,  2, -1, -1
};

static const seq_t init_vals[20] = {
    1, 1, 2, 4, 14, 39, 103, 255, 665, 1741,
    4605, 12046, 31474, 82206, 215157, 563083,
    1473635, 3855111, 10085589, 26386595
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ensure cache has capacity for indices 0..need */
static void grow_cache(size_t need)
{
    if (cache_cap > need) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        grow_cache(19);                    /* need indices 0..19 */
        for (int i = 0; i < 20; ++i) cache[i] = init_vals[i];
        cache_len = 20;
    }

    /* if n below already computed, return */
    if (n < cache_len) return cache[(size_t)n];

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) grow_cache((size_t)n);

    /* compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* i >= 20 (because i >= cache_len and cache_len is 20) */
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 20; ++k) {
            seq_t term = cache[(size_t)(i - 1 - k)];
            int c = coeffs[k];
            if (c > 0) {
                pos += (seq_t)c * term;
            } else {
                neg += (seq_t)(-c) * term;
            }
        }
        cache[(size_t)i] = pos - neg;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

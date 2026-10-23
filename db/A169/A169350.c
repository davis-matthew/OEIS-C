/*
 * A169350  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n <= 29: a(n) = (n==0?1:2) + 47 * Σ_{k=1}^{n} a(n-k) For n =
 *            30: a(30) = 1 + 47 * Σ_{k=1}^{29} a(30-k) - 1128 * a(0) For n >
 *            30: a(n) = 47 * Σ_{k=1}^{29} a(n-k) - 1128 * a(n-30)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

/* multiply seq_t by a small unsigned int, using __int128 for native ints */
#if defined(__cplusplus)
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 prod = (__int128)x * m;
        return (seq_t)prod;
    } else {
        return x * m;
    }
}
#else
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    __int128 prod = (__int128)x * m;
    return (seq_t)prod;
}
#endif

/* Cache of computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Sliding window of the last 29 terms */
static seq_t tail[29] = {0};
static size_t tail_start = 0; /* index of the oldest element in the window */
static size_t tail_count = 0; /* number of elements in the window (<=29) */
static seq_t tail_sum = 0;    /* sum of elements currently in the window */

seq_t a(idx_t n)
{
    if (n + 1 > cache_size) {
        /* grow cache */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);  /* allocation failed */
        cache = new_cache;

        /* compute terms from first missing index up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum_prev = 0;
            if (i <= 29) {
                /* sum of all previous terms */
                for (idx_t j = 0; j < i; ++j)
                    sum_prev += cache[j];
            } else {
                /* window contains exactly the previous 29 terms */
                sum_prev = tail_sum;
            }

            seq_t val = 0;
            if (i <= 29) {
                val = ((i == 0) ? (seq_t)1 : (seq_t)2);
                val += mul_small(sum_prev, 47);
            } else if (i == 30) {
                val = (seq_t)1 + mul_small(sum_prev, 47) - mul_small(cache[0], 1128);
            } else { /* i > 30 */
                val = mul_small(sum_prev, 47) - mul_small(cache[i - 30], 1128);
            }
            cache[i] = val;

            /* insert new term into the sliding window */
            if (tail_count < 29) {
                size_t pos = (tail_start + tail_count) % 29;
                tail[pos] = val;
                ++tail_count;
                tail_sum += val;
            } else {
                seq_t old = tail[tail_start];
                tail_sum += val - old;
                tail[tail_start] = val;
                tail_start = (tail_start + 1) % 29;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}

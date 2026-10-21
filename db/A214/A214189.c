/*
 * A214189  Number of 0..6 colorings of a 2X(n+1) array circular in the n+1
 *          direction with new values 0..6 introduced in row major order.
 * https://oeis.org/A214189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*a(n-1) -133*a(n-2) -3905*a(n-3) +6437*a(n-4)
 *            +125497*a(n-5) +83961*a(n-6) -931595*a(n-7) -1308936*a(n-8)
 *            +809964*a(n-9) +1218672*a(n-10) Initial values: a(1)=4, a(2)=18,
 *            a(3)=353, a(4)=7434, a(5)=198052, a(6)=5731144, a(7)=172707247,
 *            a(8)=5290686318, a(9)=163203090770, a(10)=5048858758020
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of values stored (≥10 after init) */
static size_t cache_cap = 0;   /* allocated capacity */

static void ensure_cache(size_t n) {
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* integers are given as 32‑bit signed literals */
static const int coeff[10] = {
    39,   -133,  -3905,   6437,  125497,
    83961, -931595, -1308936, 809964, 1218672
};

static const seq_t init_vals[10] = {
    4ULL, 18ULL, 353ULL, 7434ULL, 198052ULL,
    5731144ULL, 172707247ULL, 5290686318ULL,
    163203090770ULL, 5048858758020ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but safe */
    /* initialize the cache with the first 10 values */
    if (cache_len == 0) {
        ensure_cache(16);
        for (int i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_len = 10;
    }
    if ((idx_t)n <= cache_len) return cache[n - 1];

    ensure_cache(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 10; ++k) {
            /* a(i-k-1) is the required previous term */
            seq_t term = cache[(i - k - 2)];
            int c = coeff[k];
            if (c >= 0) {
                sum_pos += term * (seq_t)c;
            } else {
                sum_neg += term * (seq_t)(-c);
            }
        }
        cache[i - 1] = sum_pos - sum_neg;
    }
    cache_len = n;
    return cache[n - 1];
}

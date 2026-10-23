/*
 * A164954  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=21, a(2)=420, a(3)=8400, a(4)=168000, a(5)=3360000,
 *            a(6)=67200000, a(7)=1344000000. For n ≥ 9: a(n) =
 *            19*(a(n-1)+a(n-2)+…+a(n-7)) – 190*a(n-8). a(8) is obtained from
 *            the same recurrence with an extra +1 term: a(8) = 19*(a7+…+a1) –
 *            190*a0 + 1. This comes directly from the generating f...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init[8] = {
    (seq_t)1ULL,
    (seq_t)21ULL,
    (seq_t)420ULL,
    (seq_t)8400ULL,
    (seq_t)168000ULL,
    (seq_t)3360000ULL,
    (seq_t)67200000ULL,
    (seq_t)1344000000ULL
};

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return; /* allocation failure; caller will get garbage */
    cache = tmp;

    /* fill missing initial values (0..7) */
    for (idx_t i = cache_size; i < 8 && i <= n; ++i)
        cache[i] = init[i];

    /* compute values from old size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) /* already set to init[i] above */ continue;
        if (i == 8) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 7; ++j) sum += cache[i - j];
            cache[i] = (seq_t)19 * sum - (seq_t)190 * cache[i - 8] + (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 7; ++j) sum += cache[i - j];
            cache[i] = (seq_t)19 * sum - (seq_t)190 * cache[i - 8];
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

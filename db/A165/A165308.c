/*
 * A165308  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 14 * (a(n-1)+...+a(n-8)) - 105 * a(n-9)
   for n ≥ 10, with initial values
   a(0)=1, a(1)=16, a(2)=240, a(3)=3600, a(4)=54000,
   a(5)=810000, a(6)=12150000, a(7)=182250000,
   a(8)=2733750000, a(9)=41006249880. */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)1,      (seq_t)16,     (seq_t)240,
    (seq_t)3600,   (seq_t)54000,  (seq_t)810000,
    (seq_t)12150000, (seq_t)182250000, (seq_t)2733750000,
    (seq_t)41006249880
};

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t computed_up_to = 0xFFFFFFFF; /* marker for "not computed" */

seq_t a(idx_t n)
{
    /* Return the initial values directly for n < 10. */
    if (n < 10) return init_vals[n];

    /* Ensure the cache contains values up to n. */
    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        cache_capacity = needed;
        computed_up_to = 9;
    } else if (needed > cache_capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = needed;
    }

    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 8; ++k) sum += cache[i - k];
        if (i == 9) {
            /* Use the numerator term for n = 9: a(9) - 14*sum + 105*a(0) = 1. */
            cache[i] = (seq_t)14 * sum - (seq_t)105 * cache[0] + (seq_t)1;
        } else {
            cache[i] = (seq_t)14 * sum - (seq_t)105 * cache[i - 9];
        }
    }
    computed_up_to = n;
    return cache[n];
}

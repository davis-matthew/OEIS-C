/*
 * A118468  Numbers n for which the sum of the digits plus the sum of the
 *          digits of the square, equals the sum of the digits of the cube.
 * https://oeis.org/A118468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   s(n)+s(n^2)=s(n^3)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t *cache      = NULL;   /* array of found terms                */
static idx_t  cache_sz   = 0;      /* number of terms currently stored    */
static idx_t  cache_cap  = 0;      /* capacity of the cache array         */
static seq_t  last_num   = 0;      /* last number that was tested         */

/* Ensure the cache has at least `need` capacity */
static void ensure_cap(idx_t need) {
    if (cache_cap >= need) return;
    idx_t newcap = cache_cap ? cache_cap * 2 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

/* Initialise the cache once */
static void init_cache(void) {
    if (cache) return;
    cache = (seq_t *)malloc(32 * sizeof(seq_t));
    cache_cap = 32;
    cache_sz = 1;          /* a(1) = 0 */
    cache[0] = 0;
    last_num = 0;          /* next test will be 1 */
}

/* Return the nth term of sequence A118468 */
seq_t a(idx_t n) {
    init_cache();
    while (cache_sz < n) {
        last_num++;
        seq_t sq = last_num * last_num;
        seq_t cu = sq * last_num;
        seq_t sum_n = digit_sum(last_num);
        seq_t sum_sq = digit_sum(sq);
        seq_t sum_cu = digit_sum(cu);
        if (sum_n + sum_sq == sum_cu) {
            ensure_cap(cache_sz + 1);
            cache[cache_sz++] = last_num;
        }
    }
    return cache[n - 1];
}

/*
 * A170689  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 8 a(n) = 7 · a(n-1) for 2 ≤ n ≤ 49 a(50) = 8 ·
 *            7^49 − 28 a(n) = 6 · (a(n-1)+…+a(n-49)) − 21 · a(n-50) for n ≥
 *            51
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array storing computed a(k) values   */
static idx_t  cache_len = 0;         /* number of elements already computed */
static size_t cache_cap = 0;         /* allocated capacity                 */

/* ensure that values a(0)..a(up) are available in cache */
static void ensure_cache(idx_t up)
{
    if (cache_len > up) return;               /* already computed */

    /* enlarge storage if necessary */
    if (cache_cap == 0) cache_cap = 64;
    while (cache_cap <= (size_t)up) cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

    /* compute missing values */
    for (idx_t i = cache_len; i <= up; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)8;
        } else if (i <= 49) {
            cache[i] = cache[i-1] * (seq_t)7;
        } else if (i == 50) {
            /* 7^49 */
            seq_t pow7 = (seq_t)1;
            for (int j = 0; j < 49; ++j) pow7 *= (seq_t)7;
            cache[i] = (seq_t)8 * pow7 - (seq_t)28;
        } else {   /* i >= 51 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 49; ++j) sum += cache[i - j];
            cache[i] = sum * (seq_t)6 - cache[i-50] * (seq_t)21;
        }
    }
    cache_len = up + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

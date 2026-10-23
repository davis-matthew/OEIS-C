/*
 * A041152  Numerators of continued fraction convergents to sqrt(86).
 * https://oeis.org/A041152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=9, a(1)=28; for n>=2
   a(n) = coeff[n-1] * a(n-1) + a(n-2),
   where coeff repeats [3,1,1,1,8,1,1,1,3,18] */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static const int period_int[10] = {3, 1, 1, 1, 8, 1, 1, 1, 3, 18};

static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len) return;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)9;
        } else {
            coeff = (seq_t)period_int[(i - 1) % 10];
        }

        seq_t val;
        if (i == 0) {
            /* p0 = coeff * p_{-1} + p_{-2}  with p_{-1}=1, p_{-2}=0 */
            val = coeff * (seq_t)1 + (seq_t)0;
        } else if (i == 1) {
            /* p1 = coeff * p0 + p_{-1}  with p_{-1}=1 */
            val = coeff * cache[0] + (seq_t)1;
        } else {
            val = coeff * cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
        }

        cache[(size_t)i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

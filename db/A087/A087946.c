/*
 * A087946  Expansion of (1-3x+x^2)/((1-2x)(1-4x+x^2)).
 * https://oeis.org/A087946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 3
   a(2) = 10
   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3)   (n > 2)
*/
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_cap(idx_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 16;
    while (new_cap <= (size_t)needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        ensure_cap(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i == 1) {
                val = (seq_t)3;
            } else if (i == 2) {
                val = (seq_t)10;
            } else {
                seq_t t1 = cache[i-1] * (seq_t)6;
                seq_t t2 = cache[i-3] * (seq_t)2;
                seq_t t3 = cache[i-2] * (seq_t)9;
                val = t1 + t2 - t3;
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}

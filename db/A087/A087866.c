/*
 * A087866  Composition length of the n-th symmetric power of the natural
 *          representation of a finite subgroup of SL(2,C) of type E_8 (binary
 *          icosahedral group).
 * https://oeis.org/A087866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   (1 - x^15) / ((1 - x)(1 - x^6)(1 - x^10)) Recurrence derived:
 *            a(n) = a(n-1) + a(n-6) - a(n-7) + a(n-10) - a(n-11) - a(n-16) +
 *            a(n-17) + (n==0 ? 1 : n==15 ? -1 : 0) for n ≥ 0 with a(k) = 0
 *            for k<0
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

static void build_cache(idx_t n)
{
    size_t up = (size_t)n;
    if (up < cache_size) return;
    size_t new_size = up + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);            /* allocation failure – abort */
    cache = new_cache;
    for (size_t i = cache_size; i <= up; ++i) {
        seq_t sum = 0;
        if (i >= 1)  sum += cache[i-1];
        if (i >= 6)  sum += cache[i-6];
        if (i >= 7)  sum -= cache[i-7];
        if (i >=10)  sum += cache[i-10];
        if (i >=11)  sum -= cache[i-11];
        if (i >=16)  sum -= cache[i-16];
        if (i >=17)  sum += cache[i-17];
        if (i == 0)  sum += 1;
        else if (i == 15) sum -= 1;
        cache[i] = sum;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    build_cache(n);
    return cache[(size_t)n];
}

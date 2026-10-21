/*
 * A055997  Numbers k such that k*(k - 1)/2 is a square.
 * https://oeis.org/A055997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 6*a(n - 1) - a(n - 2) - 2 (n >= 3), a(1) = 1, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t last = 0;
static idx_t cap = 0;

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (cache == NULL) {
        /* Allocate space enough for the requested term */
        cap = (n > 2) ? n : 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        /* initialise the first two terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        last = 2;
    }
    if ((idx_t)n > last) {
        if ((idx_t)n > cap) {
            cap = n;
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }
        while (last < n) {
            /* a(k) = 6*a(k-1) - a(k-2) - 2  for k = last+1 */
            seq_t val = 6 * cache[last - 1] - cache[last - 2] - (seq_t)2;
            cache[last] = val;
            ++last;
        }
    }
    return cache[n - 1];
}

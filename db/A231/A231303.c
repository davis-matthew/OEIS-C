/*
 * A231303  Recurrence a(n) = a(n-2) + n^M for M=4, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 132, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(n)=a(n-2)+n^4
   (Equivalently a(n)=Sum_{k=0..⌊n/2⌋} (n-2k)^4) */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 8;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)0;
        else if (i == 1)
            cache[1] = (seq_t)1;
        else {
            seq_t si = (seq_t)i;
            seq_t sq1 = si * si;   /* i^2 */
            seq_t sq2 = sq1 * sq1; /* i^4 */
            cache[i] = cache[i - 2] + sq2;
        }
    }

    cache_size = n + 1;
    return cache[n];
}

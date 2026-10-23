/*
 * A026200  a(n) = (s(n) + 2)/3, where s(n) is the n-th number congruent to 1
 *          mod 3 in A026166.
 * https://oeis.org/A026200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n>8,
 *            a(1)=1,a(2)=2,a(3)=4,a(4)=6,a(5)=3,a(6)=8,a(7)=10,a(8)=12
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;           /* cached values a(1) … a(cache_size) */
static size_t cache_size = 0;         /* number of filled entries */
static size_t cache_capacity = 0;     /* allocated size of cache array */

static void grow_cache(size_t required)
{
    if (required <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
    while (new_cap < required) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */
    size_t idx = (size_t)n;

    /* Extend cache if necessary */
    if (cache_size < idx) {
        grow_cache(idx);
        for (size_t i = cache_size + 1; i <= idx; ++i) {
            seq_t val;
            if (i <= 8) {
                switch (i) {
                    case 1: val = 1; break;
                    case 2: val = 2; break;
                    case 3: val = 4; break;
                    case 4: val = 6; break;
                    case 5: val = 3; break;
                    case 6: val = 8; break;
                    case 7: val = 10; break;
                    case 8: val = 12; break;
                }
            } else {
                /* a(i) = 2*a(i-4) - a(i-8) */
                seq_t a4 = cache[i - 5];   /* cache index i-4-1 = i-5 */
                seq_t a8 = cache[i - 9];   /* cache index i-8-1 = i-9 */
                val = (seq_t)((seq_t)2 * a4 - a8);
            }
            cache[i - 1] = val;          /* store result */
        }
        cache_size = idx;
    }

    return cache[idx - 1];
}

/*
 * A196431  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,1,0,3,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 5*a(n-3) + 6*a(n-4) + 4*a(n-5) -
 *            5*a(n-6) - 5*a(n-7) - a(n-8) + a(n-9) Initial values: a(1)=3,
 *            a(2)=9, a(3)=25, a(4)=75, a(5)=213, a(6)=621, a(7)=1801,
 *            a(8)=5219, a(9)=15133
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* 1‑based index cache */
static idx_t  cache_size = 0;          /* size of allocated array (including index 0)  */
static idx_t  computed_up_to = 0;      /* highest n computed so far */

static void ensure_cache_size(idx_t needed)
{
    if (cache_size >= needed) return;
    idx_t new_size = cache_size ? cache_size << 1 : 16;
    if (new_size < needed) new_size = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) abort();           /* allocation failure, terminate */
    cache = new_cache;
    cache_size = new_size;
}

static void init_first_nine(void)
{
    if (computed_up_to >= 9) return;
    ensure_cache_size(9);
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)25;
    cache[4] = (seq_t)75;
    cache[5] = (seq_t)213;
    cache[6] = (seq_t)621;
    cache[7] = (seq_t)1801;
    cache[8] = (seq_t)5219;
    cache[9] = (seq_t)15133;
    computed_up_to = 9;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 1, but safe return */
    if (n <= 9) {
        init_first_nine();
        return cache[n];
    }

    init_first_nine();

    if (n > computed_up_to) {
        ensure_cache_size(n);
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = cache[i-1]
                       + (seq_t)3 * cache[i-2]
                       + (seq_t)5 * cache[i-3]
                       + (seq_t)6 * cache[i-4]
                       + (seq_t)4 * cache[i-5]
                       - (seq_t)5 * cache[i-6]
                       - (seq_t)5 * cache[i-7]
                       -            cache[i-8]
                       +            cache[i-9];
        }
        computed_up_to = n;
    }
    return cache[n];
}

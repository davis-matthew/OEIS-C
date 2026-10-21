/*
 * A218281  Hilltop maps: number of n X 1 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or vertical neighbor in a random 0..4 n X 1 array.
 * https://oeis.org/A218281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218281
   Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)
   (n > 9).  Initial values:
     a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=31,
     a(6)=61, a(7)=121, a(8)=241, a(9)=481
*/

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cache[0] unused, cache[1]..cache[cache_max] */
static idx_t  cache_max = 0;    /* largest computed n */
static size_t cache_cap = 0;    /* allocated array size */

/* Ensure cache can hold indices 0..n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0..n */
    if (needed <= cache_cap) return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    if (newcap < needed) newcap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
    cache = tmp;
    cache_cap = newcap;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined region */

    if (n > cache_max) {
        ensure_capacity(n);

        /* Initial values for n=1..9 */
        static const seq_t init[10] = {
            0,  /* unused for index 0 */
            1, 3, 7, 15, 31, 61, 121, 241, 481
        };

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init[i];
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 9; ++k) sum += cache[i - k];
                cache[i] = sum;
            }
        }
        cache_max = n;
    }

    return cache[n];
}

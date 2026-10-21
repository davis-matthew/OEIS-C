/*
 * A203761  a(n)=f(a(n-1)+1,a(n-2),a(n-3)), where f(x,y,z)=yz+zx+xy and
 *          a(1)=0, a(2)=0, a(3)=1.
 * https://oeis.org/A203761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 12, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=0, a(3)=1
   a(n)=f(a(n-1)+1,a(n-2),a(n-3)), where
   f(x,y,z)=yz+zx+xy.  */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

/* helpers for dynamic seq_t arrays (present in the build) */
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t)   */
#include "../utils/free_seq_array.h"    /* void free_seq_array(seq_t*)      */

static seq_t *cache      = NULL;   /* stores a(1)...a(cache_len)      */
static size_t cache_cap  = 0;      /* allocated array capacity        */
static idx_t   cache_len  = 0;      /* number of terms actually stored */

seq_t a(idx_t n)
{
    if (n == 0)                /* the OEIS offset is 1, so 0 is absurd */
        return (seq_t)0;

    /* grow the cache if necessary, doubling capacity each time */
    if ((idx_t)cache_cap < n) {
        /* new capacity: at least n, double existing size for amortization */
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < (size_t)n) new_cap = (size_t)n;

        /* allocate new array (fully constructed objects) */
        seq_t *new_arr = alloc_seq_array(new_cap);
        /* copy existing terms */
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];

        /* free old array (destructors called by free_seq_array) */
        free_seq_array(cache);

        cache = new_arr;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)0;
        } else if (i == 2) {
            cache[1] = (seq_t)0;
        } else if (i == 3) {
            cache[2] = (seq_t)1;
        } else {
            /* recurrence for i >= 4:
               a(i) = (a(i-1)+1)*a(i-2) + a(i-1)*a(i-3) + a(i-2)*a(i-3) */
            seq_t x = cache[i-2] + (seq_t)1;  /* a(i-1)+1 */
            seq_t y = cache[i-3];             /* a(i-2)   */
            seq_t z = cache[i-4];             /* a(i-3)   */
            cache[i-1] = y * z + z * x + x * y;
        }
    }
    cache_len = n;   /* now cache contains terms up to n */

    return cache[n-1];   /* cache[0] == a(1) */
}

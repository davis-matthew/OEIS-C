/*
 * A197302  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,4,0,2,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 2*a(n-1) +3*a(n-2) +3*a(n-3) +4*a(n-4) +2*a(n-5) +10*a(n-6) +6*a(n-7)
          -7*a(n-8) -23*a(n-9) -27*a(n-10) -23*a(n-11) +5*a(n-12) +29*a(n-13) +15*a(n-14) +2*a(n-15)
   for n > 15, with initial values
     a(1)=2, a(2)=10, a(3)=31, a(4)=98, a(5)=344, a(6)=1139,
     a(7)=3750, a(8)=12470, a(9)=41479, a(10)=137872,
     a(11)=458412, a(12)=1524061, a(13)=5066480,
     a(14)=16842888, a(15)=55992885
   The offset is 1 (a(1) is the first term). */
#include <stdlib.h>

static const seq_t init_values[15] = {
    2, 10, 31, 98, 344, 1139, 3750, 12470, 41479, 137872,
    458412, 1524061, 5066480, 16842888, 55992885
};

static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t cached_up_to = 0;

static void ensure_cache(idx_t n) {
    if (capacity >= n)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (!tmp)
        abort();
    cache = tmp;
    capacity = n;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* undefined for n < 1, return 0 as placeholder */
    ensure_cache(n);
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i <= 15) {
            cache[i - 1] = init_values[i - 1];
        } else {
            seq_t val = 0;
            /* Add positive contributions */
            val += cache[i - 2] * 2 + cache[i - 3] * 3 + cache[i - 4] * 3
                + cache[i - 5] * 4 + cache[i - 6] * 2 + cache[i - 7] * 10
                + cache[i - 8] * 6;
            /* Subtract negative contributions */
            val -= cache[i - 9] * 7 + cache[i - 10] * 23 + cache[i - 11] * 27
                  + cache[i - 12] * 23;
            /* Add remaining positive terms */
            val += cache[i - 13] * 5 + cache[i - 14] * 29 + cache[i - 15] * 15
                  + cache[i - 16] * 2;
            cache[i - 1] = val;
        }
    }
    cached_up_to = n;
    return cache[n - 1];
}

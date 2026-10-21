/*
 * A230270  Number of n X 3 0..2 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 3, and upper left element zero.
 * https://oeis.org/A230270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A230270
   a(n) = 11*a(n-1) - 18*a(n-2) - 22*a(n-3) - 6*a(n-4)
          + 162*a(n-5) - 29*a(n-6) - 105*a(n-7)
          - 62*a(n-8) - 22*a(n-9) - 4*a(n-10)   (n ≥ 11)
   Initial values:
     a(1) = 0
     a(2) = 2
     a(3) = 28
     a(4) = 258
     a(5) = 2318
     a(6) = 19996
     a(7) = 173042
     a(8) = 1495602
     a(9) = 12923884
     a(10) = 111679970
*/

#include <stdbool.h>

#define MAX_N 20000   /* enough for n up to about 10000 */

static seq_t cache[MAX_N + 1];
static bool cache_initialized = false;
static idx_t cache_filled_upto = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise base values once */
    if (!cache_initialized) {
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 28;
        cache[4] = 258;
        cache[5] = 2318;
        cache[6] = 19996;
        cache[7] = 173042;
        cache[8] = 1495602;
        cache[9] = 12923884;
        cache[10] = 111679970;
        cache_filled_upto = 10;
        cache_initialized = true;
    }

    /* extend cache if needed */
    if (n > MAX_N) {
        /* sequence defined only up to MAX_N in this implementation */
        return 0;
    }

    if (n <= cache_filled_upto) return cache[n];

    /* compute missing terms using the empirical recurrence */
    for (idx_t i = cache_filled_upto + 1; i <= n; ++i) {
        seq_t next = 
            (seq_t)11 * cache[i-1]
          - (seq_t)18 * cache[i-2]
          - (seq_t)22 * cache[i-3]
          - (seq_t) 6 * cache[i-4]
          + (seq_t)162 * cache[i-5]
          - (seq_t)29 * cache[i-6]
          - (seq_t)105 * cache[i-7]
          - (seq_t)62 * cache[i-8]
          - (seq_t)22 * cache[i-9]
          - (seq_t) 4 * cache[i-10];
        cache[i] = next;
    }
    cache_filled_upto = n;
    return cache[n];
}

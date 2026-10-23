/*
 * A188102  Number of (n+3) X 9 binary arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A188102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 60, u64 n <= 150, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence:
   for n > 16
   a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) + a(n-8) - a(n-9)

   Initial values:
   a(1) = 13814,  a(2) = 2374,  a(3) = 3765,  a(4) = 5661,
   a(5) = 8509,   a(6) = 10229, a(7) = 10653, a(8) = 11245,
   a(9) = 12257,  a(10)=14405, a(11)=17321, a(12)=21217,
   a(13)=28861,  a(14)=38625, a(15)=49581, a(16)=64510
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[16] = {
        13814, 2374, 3765, 5661, 8509, 10229, 10653, 11245,
        12257, 14405, 17321, 21217, 28861, 38625, 49581, 64510
    };

    if (n == 0) return 0;          /* not used, but safe */
    if (n <= 16) return init[n-1];

    /* allocate array for 1..n */
    seq_t *arr = (seq_t *)malloc(n * sizeof(seq_t));
    if (!arr) return 0; /* allocation failed */

    /* copy initial values */
    for (idx_t i = 0; i < 16; ++i) arr[i] = init[i];

    /* compute terms from n=17 upwards: index i corresponds to a(i+1) */
    for (idx_t i = 16; i < n; ++i) {
        arr[i] = 2*arr[i-1] - arr[i-2] + arr[i-4] - 2*arr[i-5]
                 + arr[i-6] + arr[i-8] - arr[i-9];
    }

    seq_t result = arr[n-1];
    free(arr);
    return result;
}

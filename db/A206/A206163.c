/*
 * A206163  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A206163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 217*a(n-2) - 33*a(n-3) - 1320*a(n-4) -
 *            432*a(n-5) + 1152*a(n-6) for n > 7 with a(1)=85, a(2)=1070,
 *            a(3)=17142, a(4)=238182, a(5)=3814022, a(6)=53494742,
 *            a(7)=850283062
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last = 0;

    if (n < 1)
        return (seq_t)0;

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = (cache_size == 0) ? 8 : cache_size * 2;
        if (new_size <= n)
            new_size = n + 1;
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation failed */
        cache = (seq_t *)tmp;
        cache_size = new_size;
    }

    /* compute values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[1] = (seq_t)85;                 break;
            case 2: cache[2] = (seq_t)1070;              break;
            case 3: cache[3] = (seq_t)17142;             break;
            case 4: cache[4] = (seq_t)238182;            break;
            case 5: cache[5] = (seq_t)3814022;           break;
            case 6: cache[6] = (seq_t)53494742;          break;
            case 7: cache[7] = (seq_t)850283062;         break;
            default: {
                seq_t val = cache[i - 1];
                val += cache[i - 2] * (seq_t)217;
                val -= cache[i - 3] * (seq_t)33;
                val -= cache[i - 4] * (seq_t)1320;
                val -= cache[i - 5] * (seq_t)432;
                val += cache[i - 6] * (seq_t)1152;
                cache[i] = val;
            }
        }
    }

    last = n;
    return cache[n];
}

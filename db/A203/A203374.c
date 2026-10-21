/*
 * A203374  Number of (n+1) X 5 0..1 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements.
 * https://oeis.org/A203374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) -65*a(n-2) +92*a(n-3) -48*a(n-4) +8*a(n-5) a(1)
 *            = 324, a(2) = 3412, a(3) = 36300, a(4) = 387324, a(5) = 4136292
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* The sequence is needed for n up to about 10 000.  A small static array
       keeps all computed values and guarantees O(1) retrieval after the
       initial computation for that n.  The size 20 001 is safely above the
       required maximum.  */
    enum { MAXN = 20001 };
    static seq_t cache[MAXN];
    static size_t cached_len = 0;   /* number of terms stored (a(1)..a(cached_len)) */

    if (n == 0) return 0;          /* offset is 1, calling with 0 is unused */

    /* Return already stored value if available */
    if ((size_t)n <= cached_len)
        return cache[n - 1];

    /* Initialise the first five terms when they become needed */
    for (size_t i = cached_len + 1; i <= n && i <= 5; ++i) {
        switch (i) {
        case 1: cache[0] = (seq_t)324;            break;
        case 2: cache[1] = (seq_t)3412;           break;
        case 3: cache[2] = (seq_t)36300;          break;
        case 4: cache[3] = (seq_t)387324;         break;
        case 5: cache[4] = (seq_t)4136292;        break;
        }
    }

    /* Compute new terms using the recurrence.  The recurrence involves
       addition and subtraction but never negative results for valid n. */
    size_t start = cached_len + 1;
    if (start < 6) start = 6;      /* recurrence starts at n = 6 */

    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t term = mul_small(cache[i - 2], 16);
        term = term - mul_small(cache[i - 3], 65);
        term = term + mul_small(cache[i - 4], 92);
        term = term - mul_small(cache[i - 5], 48);
        term = term + mul_small(cache[i - 6], 8);
        cache[i - 1] = term;
    }

    cached_len = n;
    return cache[n - 1];
}

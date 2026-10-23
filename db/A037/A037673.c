/*
 * A037673  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 3,2,0.
 * https://oeis.org/A037673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + digit(n), where digit(n) repeats the cycle
 *            3,2,0. a(1) = 3. This implementation memoises computed terms so
 *            that arbitrary order of calls is supported
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1 – ignore n < 1 (not defined) */
    if (n < 1) return 0;

    /* static memoisation buffer */
    static seq_t *cache   = NULL;
    static idx_t computed = 0; /* number of terms stored so far */
    static idx_t cap      = 0; /* allocated capacity */

    /* grow the buffer if necessary */
    if (cap < n) {
        idx_t newcap = cap ? cap << 1 : 1;
        while (newcap < n) newcap <<= 1;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute any missing terms */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t digit;
            switch ((i - 1) % 3) { /* cycle 0->3, 1->2, 2->0 */
                case 0: digit = 3; break;
                case 1: digit = 2; break;
                default: digit = 0; break;
            }
            if (i == 1) {
                cache[0] = 3;
            } else {
                cache[i - 1] = cache[i - 2] * 10 + digit;
            }
        }
        computed = n;
    }

    return cache[n - 1];
}

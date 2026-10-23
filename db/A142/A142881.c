/*
 * A142881  a(0) = 0, a(1) = 1, after which, if n=3k: a(n) = 2*a(n-1) -
 *          a(n-2), if n=3k+1: a(n) = a(n-1) + a(n-2), if n=3k+2: a(n) =
 *          2*a(n-1) + a(n-2).
 * https://oeis.org/A142881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1 for n ≥ 2: if n % 3 == 0: a(n) = 2*a(n-1) -
 *            a(n-2) if n % 3 == 1: a(n) = a(n-1) + a(n-2) if n % 3 == 2: a(n)
 *            = 2*a(n-1) + a(n-2)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 70, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* a(0) and a(1) */
    seq_t prevprev = 0; /* a(n-2) */
    seq_t prev = 1;     /* a(n-1) */
    seq_t curr = 0;     /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (i % 3 == 0)        /* n = 3k   */
            curr = prev * 2 - prevprev;
        else if (i % 3 == 1)   /* n = 3k+1 */
            curr = prev + prevprev;
        else                   /* n = 3k+2 */
            curr = prev * 2 + prevprev;

        /* shift the window for the next iteration */
        prevprev = prev;
        prev = curr;
    }

    return prev;
}

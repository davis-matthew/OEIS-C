/*
 * A018472  Divisors of 470.
 * https://oeis.org/A018472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 470 in increasing order (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                  /* offset is 1 */
        return 0;

    const seq_t N = (seq_t)470;
    idx_t  cnt = 0;

    for (seq_t d = (seq_t)1; d <= N; ++d)
    {
        if ((N % d) == (seq_t)0)   /* d is a divisor of 470 */
        {
            ++cnt;
            if (cnt == n)
                return d;
        }
    }
    /* n larger than the number of divisors (8) */
    return (seq_t)0;
}

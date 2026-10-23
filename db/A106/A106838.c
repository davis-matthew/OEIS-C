/*
 * A106838  Numbers m such that m, m+1 and m+2 have odd part of the form
 *          4*k+3.
 * https://oeis.org/A106838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*A091067(n) - 2 (OEIS A106838)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   count_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, if needed elsewhere */
#include "../utils/count_le.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* sequence is undefined for n=0 */

    seq_t target = (seq_t)n;     /* nth element of A091067 */

    /* Find a binary search interval where the count exceeds target */
    seq_t lo = 0;
    seq_t hi = 1;
    while (count_le(hi) < target)
        hi <<= 1;                /* double until sufficient */

    /* Binary search for the smallest m with count_le(m) >= target */
    while (lo < hi) {
        seq_t mid = (lo + hi) >> 1;
        if (count_le(mid) >= target)
            hi = mid;
        else
            lo = mid + 1;
    }

    seq_t m = lo;                /* m is the nth number with odd part ≡ 3 (mod 4) */
    return m * 8 - 2;            /* transform to A106838 */
}

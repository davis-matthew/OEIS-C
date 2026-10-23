/*
 * A145264  a(n) is the positive integer such that Sum_{k>=0}
 *          floor(n/(a(n)+k)) = n, or 0 if there is no such positive integer.
 * https://oeis.org/A145264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer a such that Sum_{k=0}^{n-a}
 *            floor(n/(a+k)) = n; if no such integer exists, a(n)=0
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   sum_floors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sum_floors.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;

    idx_t low  = 1;
    idx_t high = n;
    seq_t ans  = 0;

    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        seq_t s   = sum_floors(n, mid);

        if (s == (seq_t)n)
        {
            ans = (seq_t)mid;
            break;
        }
        else if (s > (seq_t)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return ans;
}

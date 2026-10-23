/*
 * A161398  a(n) = the smallest divisor of n that contains the same number of
 *          1's as n when both numbers are written in binary.
 * https://oeis.org/A161398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest divisor d of n such that popcount(d) ==
 *            popcount(n)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is indexed starting at 1.  For any n ≥ 1, this function returns the smallest
   divisor of n whose binary representation contains the same number of 1's as n
   itself.  The implementation is O(√n) per call and uses only integer
   arithmetic. */
#include <stdint.h>

static int popcount_u64(uint64_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - 1;  /* clear the least significant 1 bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* The sequence definition guarantees that the answer is always <= n, so
       it fits into idx_t which is uint64_t.  We cast the found divisor to
       seq_t when returning. */
    if (n == 0) /* offset starts at 1; n==0 is undefined but return 0 safely */
        return (seq_t)0;

    uint64_t un = (uint64_t)n;
    int target_pop = popcount_u64(un);
    seq_t ans = 0;

    for (uint64_t d = 1; d * d <= un; ++d)
    {
        if (un % d == 0)
        {
            if (popcount_u64(d) == target_pop)
            {
                if (ans == 0 || d < ans)
                    ans = (seq_t)d;
            }
            uint64_t d2 = un / d;
            if (d2 != d && popcount_u64(d2) == target_pop)
            {
                if (ans == 0 || d2 < ans)
                    ans = (seq_t)d2;
            }
        }
    }

    /* The divisor n itself always satisfies popcount(n) == popcount(n).
       If ans is still 0 (should not happen), use n. */
    if (ans == 0)
        ans = (seq_t)un;

    return ans;
}

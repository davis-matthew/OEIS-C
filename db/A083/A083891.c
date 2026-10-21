/*
 * A083891  Number of divisors of n with largest digit = 4 (base 10).
 * https://oeis.org/A083891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n whose decimal representation
 *            has 4 as its largest digit (i.e. all digits ≤ 4 and at least one
 *            digit equal to 4)
 * Range:     u8 n <= 2190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

static bool has_largest_digit4(seq_t v)
{
    unsigned char cnt[10] = {0};
    digit_counts(v, cnt);
    for (int d = 5; d < 10; ++d)
        if (cnt[d]) return false;   /* a digit > 4 appears */
    return cnt[4] != 0;             /* at least one digit == 4 */
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)                 /* i is a divisor of n */
        {
            seq_t d1 = (seq_t)i;
            if (has_largest_digit4(d1))
                ++count;

            idx_t j = n / i;
            if (j != i)                  /* avoid double-counting sqrt(n) */
            {
                seq_t d2 = (seq_t)j;
                if (has_largest_digit4(d2))
                    ++count;
            }
        }
    }
    return count;
}

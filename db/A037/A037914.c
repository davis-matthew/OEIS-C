/*
 * A037914  a(n) is the least base b>=2 such that the number of distinct
 *          digits in the base b representation of n is maximized.
 * https://oeis.org/A037914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search over bases 2..n
   to find the least base that maximises the number of distinct
   digits in the base‑b representation of n. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the exceptional case n == 1 */
    if (n == 1) return (seq_t)2;

    seq_t best_base = 2;
    int best_distinct = 0;

    /* Temporary storage for the digits of n in base b.
       The maximum number of digits for n ≤ 10 000 is less than 20,
       so an array of size 64 is more than enough. */
    int digits[64];

    for (idx_t b = 2; b <= n; ++b)
    {
        /* Convert n to base b, collecting digits. */
        idx_t temp = n;
        int len = 0;
        while (temp > 0)
        {
            digits[len++] = (int)(temp % b);
            temp /= b;
        }

        /* Count distinct digits. */
        int distinct = 0;
        for (int i = 0; i < len; ++i)
        {
            int d = digits[i];
            int j;
            for (j = 0; j < i; ++j)
                if (digits[j] == d) break;
            if (j == i) ++distinct;
        }

        /* Keep the smallest base if this is a new maximum. */
        if (distinct > best_distinct)
        {
            best_distinct = distinct;
            best_base = b;
        }
    }

    return best_base;
}

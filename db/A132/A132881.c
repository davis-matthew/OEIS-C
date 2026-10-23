/*
 * A132881  a(n) is the number of isolated divisors of n.
 * https://oeis.org/A132881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isolated divisors of n (definition in the OEIS
 *            entry)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t sqrt_n = 0;
    /* compute floor(sqrt(n)) without using floating-point */
    while ((sqrt_n + 1) <= n / (sqrt_n + 1))
        ++sqrt_n;

    seq_t count = 0;

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0)
            continue;

        /* check divisor d itself */
        bool isolated_small;
        if (d == 1)
            isolated_small = (n & 1ULL);        /* 1 isolated iff n is odd */
        else {
            if (n % (d - 1) == 0)
                isolated_small = false;
            else if (n % (d + 1) == 0)
                isolated_small = false;
            else
                isolated_small = true;
        }
        if (isolated_small)
            ++count;

        idx_t other = n / d;
        if (other == d)            /* avoid double counting a square root */
            continue;

        /* check the paired divisor other */
        bool isolated_large;
        if (other == 1)
            isolated_large = (n & 1ULL);
        else {
            if (n % (other - 1) == 0)
                isolated_large = false;
            else if (other != n && n % (other + 1) == 0)
                isolated_large = false;
            else
                isolated_large = true;
        }
        if (isolated_large)
            ++count;
    }

    return count;
}

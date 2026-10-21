/*
 * A211187  a(n) = smallest r>=2 such that the digits of n in base r form a
 *          permutation of {0,1,...,s-1} for some s<=r.
 * https://oeis.org/A211187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest r >= 2 such that the digits of n in base r are a
 *            permutation of {0,1,...,s-1} for some s <= r
 * Range:     u8 n <= 259, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


/* Computes the A211187 sequence term for index n. */
seq_t a(idx_t n)
{
    /* Handle n = 0 directly; its representation in any base is just 0. */
    if (n == 0)
        return (seq_t)2;

    /* Search for the smallest base r. The answer is guaranteed to be ≤ n
       (since base n representation is 10 which satisfies the condition). */
    for (idx_t r = 2; ; ++r)
    {
        idx_t temp = n;
        idx_t digits[64];   /* Enough to hold all digits (max ~ log2(n) + 1). */
        unsigned len = 0;   /* Number of distinct digits seen. */
        idx_t max_digit = 0;
        bool has0 = false;
        bool bad = false;

        /* Extract digits of n in base r, checking for duplicates. */
        while (temp > 0)
        {
            idx_t d = temp % r;
            temp /= r;

            /* Check if digit d was already seen. */
            unsigned i;
            for (i = 0; i < len; ++i)
                if (digits[i] == d)
                {
                    bad = true;
                    break;
                }
            if (bad)
                break;

            digits[len++] = d;
            if (d > max_digit)
                max_digit = d;
            if (d == 0)
                has0 = true;
        }

        if (bad)          /* Duplicate digit, cannot use this base. */
            continue;

        /* Evaluate the permutation condition with possible leading zero. */
        if (has0)                     /* Zero is already present. */
        {
            if (len == (unsigned)(max_digit + 1))
                return (seq_t)r;
        }
        else                           /* Add a single leading zero. */
        {
            if (len == (unsigned)max_digit)
                return (seq_t)r;
        }
    }
}

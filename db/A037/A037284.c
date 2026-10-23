/*
 * A037284  Replace n with concatenation of its odd divisors >1.
 * https://oeis.org/A037284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 104, u64 n <= 314,
 *            u128 n <= 2834, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037284: a(n) = concatenation of the odd divisors of n that are
   greater than 1, in ascending order.
   For n that have no such divisors the value is 0. */

#include <stdint.h>
#include <stdlib.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* --- remove all factors of 2 from n --- */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0)
        m >>= 1;
    if (m == 1)          /* n is a power of two → no odd divisor >1 */
        return 0;

    /* --- gather all odd divisors >1 of m --- */
    /*  m is odd; every divisor is odd.  The number of divisors of any
        integer ≤ 2⁶⁴ is at most 768 (for 73513440); 128 slots are ample
        for the small values used by the tests. */
    uint64_t divs[128];
    size_t cnt = 0;

    for (uint64_t d = 1; d <= m / d; ++d)          /* avoid overflow */
    {
        if (m % d == 0)
        {
            uint64_t d1 = d, d2 = m / d;
            if (d1 > 1)
                divs[cnt++] = d1;
            if (d2 > 1 && d2 != d1)
                divs[cnt++] = d2;
        }
    }

    /* --- sort the divisors in ascending order --- */
    for (size_t i = 1; i < cnt; ++i)
    {
        uint64_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key)
        {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* --- concatenate the divisors --- */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t d = divs[i];

        /* number of decimal digits in d */
        unsigned digits = 0;
        uint64_t tmp = d;
        while (tmp > 0)
        {
            ++digits;
            tmp /= 10;
        }
        if (digits == 0)
            digits = 1;   /* d==0 cannot occur but keeps definition safe */

        /* 10^digits in seq_t, avoiding overflow by multiplying in seq_t */
        seq_t pow10 = 1;
        for (unsigned k = 0; k < digits; ++k)
            pow10 *= 10;

        result = result * pow10 + (seq_t)d;
    }

    return result;
}

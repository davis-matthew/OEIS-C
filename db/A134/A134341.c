/*
 * A134341  Numbers whose fifth powers have a partition as a sum of fifth
 *          powers of four positive integers.
 * https://oeis.org/A134341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Union of multiples of 144 and 85359
 * a(n) = minimal x such that floor(x/144) + floor(x/85359) - floor(x/LCM) >= n
 * where LCM = lcm(144, 85359) = 4097232.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) is undefined, return 0 for safety */

    const seq_t LCM = (seq_t)4097232;     /* lcm(144, 85359) */
    seq_t target = (seq_t)n;

    /* Lower and upper bounds for binary search.
       The upper bound is large enough because the density of the union
       is at least 1/144, so multiplying n by 85359 guarantees enough room. */
    seq_t low  = 1;
    seq_t high = target * (seq_t)85359;

    while (low < high)
    {
        seq_t mid = low + ((high - low) >> 1);
        seq_t count = (mid / (seq_t)144) + (mid / (seq_t)85359) - (mid / LCM);
        if (count >= target)
            high = mid;
        else
            low = mid + 1;
    }
    return low;   /* the n-th term of the sequence */
}

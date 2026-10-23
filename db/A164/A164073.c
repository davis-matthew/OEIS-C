/*
 * A164073  a(n) = 2*a(n-2) for n > 2; a(1) = 1, a(2) = 3.
 * https://oeis.org/A164073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^((n-1)/2) if n is odd, a(n) = 3 * 2^((n/2)-1) if n is
 *            even
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the recurrence a(n) = 2*a(n-2), a(1)=1, a(2)=3. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /*  n >= 1 is assumed. */
    if (n & 1ULL) {                /* odd n : a(n) = 2^((n-1)/2) */
        idx_t exp = (n - 1) >> 1;  /* (n-1)/2 */
        return pow2(exp);
    } else {                       /* even n : a(n) = 3*2^((n/2)-1) */
        idx_t exp = (n >> 1) - 1;  /* (n/2)-1 */
        return (seq_t)3 * pow2(exp);   /* multiplication in seq_t */
    }
}

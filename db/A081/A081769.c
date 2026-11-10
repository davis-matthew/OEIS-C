/*
 * A081769  a(n)-th term of the continued fraction for 2*Sum_{k>=0} 1/2^(2^k)
 *          is 2.
 * https://oeis.org/A081769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 5*n + 3*A073089(n+1)
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A073089_a(idx_t n)
{
    if (n == 1U)
        return (seq_t)0;
    for (;;) {
        if ((n & 3ULL) == 0)
            return (seq_t)1;
        if ((n & 3ULL) == 2)
            return (seq_t)0;
        unsigned mod8 = (unsigned)(n & 7ULL);
        if (mod8 == 3)
            return (seq_t)1;
        if (mod8 == 7)
            return (seq_t)0;
        unsigned mod16 = (unsigned)(n & 15ULL);
        if (mod16 == 5)
            return (seq_t)1;
        if (mod16 == 13)
            return (seq_t)0;
        n = ((n - 1ULL) >> 1) + 1ULL;
    }
}

seq_t a(idx_t n)
{
    return (seq_t)5 * (seq_t)n + (seq_t)3 * A073089_a(n + 1);
}

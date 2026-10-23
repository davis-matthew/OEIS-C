/*
 * A021672  Decimal expansion of 1/668.
 * https://oeis.org/A021672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/668 * (a(0) is the first
 *            digit after the point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The remainder r = (10^n) mod 668 does not need to be larger than 667 */
    uint64_t r = 1;          /* corresponds to 10^0 mod 668 */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10ULL;          /* advance one digit */
        uint64_t digit = r / 668ULL;   /* the current decimal digit */
        r %= 668ULL;         /* keep only the remainder for next iteration */
        if (i == n)
            return (seq_t)digit;
    }
    /* Should never reach here */
    return (seq_t)0;
}

/*
 * A021315  Decimal expansion of 1/311.
 * https://oeis.org/A021315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/311
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=0, a(2)=3, a(3)=2, a(4)=1, a(5)=5, ... */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remainder after each step; fits in 64 bits because 311 < 2^10 */
    uint64_t rem = 1;

    /* Perform long division n+1 times to reach the n-th digit. */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;          /* bring down next digit (multiply by 10)   */
        uint64_t digit = rem / 311ULL;  /* digit at this position            */
        rem %= 311ULL;         /* new remainder for next iteration       */

        if (i == n)
            return (seq_t)digit;
    }

    /* The loop always returns; this line is for completeness. */
    return (seq_t)0;
}

/*
 * A115036  Even terms of A116883.
 * https://oeis.org/A115036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115036: 
   a(n) is the n-th even positive integer n such that
   the largest odd divisor of n is greater than the largest
   power of 2 dividing n.  That is, for n = 2^k * m (m odd), 
   we require m > 2^k.  The sequence starts 6,10,14,18,20,22,... */
#include <stdbool.h>

/* Helper that tests whether an even number x satisfies the condition. */
static bool good_even(seq_t x)
{
    seq_t odd = x;
    seq_t power2 = 1;
    /* Extract odd part and corresponding power of two. */
    while ((odd & 1) == 0) {
        odd >>= 1;
        power2 <<= 1;
    }
    return odd > power2;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    /* Count of qualifying numbers found so far. */
    idx_t count = 0;
    /* Current even candidate to test. */
    seq_t candidate = 2;

    /* Keep searching until we hit the n-th qualifying number. */
    for (;;) {
        if (good_even(candidate)) {
            ++count;
            if ((idx_t)count == n)
                return candidate;
        }
        candidate += 2;
    }
}

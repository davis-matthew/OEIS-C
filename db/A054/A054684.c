/*
 * A054684  Numbers whose sum of digits is odd.
 * https://oeis.org/A054684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth positive integer whose decimal digit sum is odd. */
/* The implementation simply enumerates positive integers and tests the
   parity of the sum of their digits.  It uses the helper function
   digit_counts to count decimal digits of an arbitrary-precision integer. */

#include "../utils/digit_counts.h"

extern seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 3, ...  */
    if (n == 0)
        return (seq_t)0;       /* undefined for offset 1, but safe */

    idx_t found = 0;           /* how many qualifying numbers seen */
    seq_t x = (seq_t)1;        /* current integer to test */

    while (1) {
        unsigned char cnt[10] = {0};
        digit_counts(x, cnt);  /* count decimal digits 0..9 of x */

        /* odd digits are 1,3,5,7,9.  Sum their counts and test parity. */
        int odd_parity = (cnt[1] + cnt[3] + cnt[5] + cnt[7] + cnt[9]) & 1;
        if (odd_parity) {
            ++found;
            if (found == n)
                return x;
        }
        x += (seq_t)1;         /* go to next integer */
    }
}

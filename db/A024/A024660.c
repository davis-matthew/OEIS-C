/*
 * A024660  n written in fractional base 10/5.
 * https://oeis.org/A024660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n written in fractional base 10/5
 * Range:     u8 n <= 19, u16 n <= 159, u32 n <= 2559, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return (seq_t)0;

    /* digits[i] holds the decimal digit for the place 2^i in the
       10/5 (i.e. base 2) representation.  Only values 0..9 occur.
       The array is sized large enough for all n up to at least 2^64. */
    seq_t digits[128];
    for (int i = 0; i < 128; ++i) digits[i] = 0;

    /* Highest index that has a non‑zero digit */
    int maxIdx = -1;

    /* Work with the index value, which is unsigned 64‑bit */
    idx_t rem = n;

    /* While the remainder is at least 5, place a digit 5 in the
       highest possible position (i.e. the largest power of 2 for which
       5 * 2^k <= rem). */
    while (rem >= 5) {
        seq_t pow = 5;          /* 5 * 2^k */
        int k = 0;              /* current exponent */

        /* Find the largest k such that 5 * 2^k <= rem. */
        while ((pow << 1) <= (seq_t)rem) {
            pow <<= 1;
            ++k;
        }

        /* Record the digit and update the remainder. */
        digits[k] += 5;
        if (k > maxIdx) maxIdx = k;
        rem -= (idx_t)pow;
    }

    /* The remaining part is < 5.  It can be stored in the lowest
       position (2^0).  Adding it cannot raise the digit above 9
       because the digit there is either 0 or 5. */
    digits[0] += (seq_t)rem;
    if (digits[0] != 0 && 0 > maxIdx) maxIdx = 0;

    /* Assemble the decimal integer from the digit array. */
    seq_t result = 0;
    for (int i = maxIdx; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}

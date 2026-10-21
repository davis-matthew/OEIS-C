/*
 * A067113  Let N = 0.149162536496481100121... = A001191, the concatenation of
 *          the squares. Then a(n) = sum of first n digits of N.
 * https://oeis.org/A067113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of the first n decimal digits of the infinite
 *            concatenation 1²,2²,3²,4²,… (OEIS A067113)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* a(n) returns the cumulative sum of the first n digits of the
   decimal expansion formed by appending the squares of the
   positive integers.  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */

    seq_t sum = (seq_t)0;
    idx_t remaining = n;

    for (idx_t i = 1; remaining > 0; ++i) {
        uint64_t sq = (uint64_t)i * (uint64_t)i;
        uint64_t tmp = sq;

        /* Extract digits of sq into an array, least significant
           digit first.  A 64‑bit integer has at most 20 decimal digits. */
        int digits[21] = {0};
        int len = 0;

        if (tmp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (tmp > 0) {
                digits[len++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Number of digits to consume from this square. */
        int take = remaining < (idx_t)len ? (int)remaining : len;

        /* Add the most significant 'take' digits. */
        for (int j = len - 1; j >= len - take; --j) {
            sum += (seq_t)digits[j];
        }

        remaining -= take;
    }

    return sum;
}

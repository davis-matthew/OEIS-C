/*
 * A054634  Champernowne sequence: write n in base 8 and juxtapose.
 * https://oeis.org/A054634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequ. A054634: Champernowne sequence in base 8.
 * a(n) is the n-th digit (0‑based) of the concatenation
 * of the non‑negative integers written in base 8.
 *
 * Formula: a(n) = digit of the integer ⌊k/⌈digits⌉⌋
 *            where k = n - 8 shifted into groups of numbers of equal base‑8 length.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* First group: numbers 0..7 (8 digits) */
    if (n < 8u) {
        return (seq_t)n;              /* digits 0..7 */
    }

    n -= 8u;                          /* skip first group */

    /* Iterate over lengths k = 2,3,... */
    idx_t k = 2u;
    for (;;) {
        /* Compute 8^(k-1) */
        seq_t pow8 = 1;
        for (idx_t i = 1; i < k; ++i) {
            pow8 *= 8u;
        }

        /* Numbers in this group: from 8^(k-1) to 8^k - 1  */
        seq_t nums_in_group = 7u * pow8;          /* 7 * 8^(k-1) */
        idx_t  group_digits = (idx_t)nums_in_group * k;

        if (n < group_digits) {
            /* n lies in this group */
            idx_t  offset       = n;             /* position inside group */
            idx_t  number_index = offset / k;     /* which number in this group */
            idx_t  digit_index  = offset % k;     /* which digit of the number */

            seq_t number = pow8 + (seq_t)number_index; /* the actual number */

            /* Convert `number` to base‑8 digits (most significant first) */
            seq_t digits[64];   /* k ≤ 64 for all practical n */
            seq_t tmp = number;
            for (idx_t i = k - 1; (int)i >= 0; --i) {
                digits[i] = tmp % 8u;
                tmp      /= 8u;
            }
            return digits[digit_index];
        }

        /* Skip this group and continue */
        n -= group_digits;
        ++k;
    }
}

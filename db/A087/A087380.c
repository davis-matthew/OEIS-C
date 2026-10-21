/*
 * A087380  Let Pricom(n) be defined as the number obtained by replacing each
 *          prime digit (2,3,5,7) of n with a '0' and a composite digit
 *          (0,4,6,8,9) with a '1' . A 1 remains the same. a(n) = Pricom(n).
 * https://oeis.org/A087380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number obtained by replacing each prime digit of n
 *            (2,3,5,7) with 0, each composite digit (0,4,6,8,9) with 1,
 *            leaving the digit 1 unchanged
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Example:  a(4206) = 1011, a(10235479) = 11000101. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint8_t digits[32];
    int len = 0;
    uint64_t tmp = (uint64_t)n;  /* n fits in uint64_t as idx_t is uint64_t */

    /* Extract decimal digits from least significant to most significant */
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Build the result in most–to–least significant order */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        uint8_t mapped = (d == 2 || d == 3 || d == 5 || d == 7) ? 0 : 1;
        res = res * 10 + mapped;
    }
    return res;
}

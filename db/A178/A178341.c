/*
 * A178341  Smallest multiple of 3 such that decimals digits 1, ..., k (k = 1,
 *          ..., 9) and 0 appear in any order.
 * https://oeis.org/A178341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   a(n) = smallest multiple of 3 that contains each digit 1..n at
 *            least once (for n ≥ 10 it is the smallest multiple of 3 that
 *            contains each digit 0..9 once)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t build_number(const unsigned char *digits, size_t len) {
    seq_t n = 0;
    for (size_t i = 0; i < len; ++i)
        n = n * 10 + (seq_t)digits[i];
    return n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, return 0 for safety */

    /* n <= 9 : require digits 1..n */
    if (n <= 9) {
        unsigned char digits[10];
        unsigned char cnt = 0;
        seq_t sum = 0;

        for (uint8_t d = 1; d <= (uint8_t)n; ++d) {
            digits[cnt++] = d;
            sum += d;
        }

        uint8_t r = (uint8_t)(sum % 3);
        if (r == 1) {
            digits[cnt++] = 2;          /* add one '2' makes sum divisible by 3 */
        } else if (r == 2) {
            digits[cnt++] = 1;          /* add one '1' makes sum divisible by 3 */
        }

        /* sort digits ascending (simple insertion sort, small size) */
        for (unsigned i = 1; i < cnt; ++i) {
            unsigned j = i;
            unsigned char key = digits[i];
            while (j > 0 && digits[j - 1] > key) {
                digits[j] = digits[j - 1];
                --j;
            }
            digits[j] = key;
        }

        return build_number(digits, cnt);
    }

    /* n == 10 : require digits 0..9 once each, with no leading 0 */
    if (n == 10) {
        /* minimal arrangement: 1 0 2 3 4 5 6 7 8 9 */
        const unsigned char digs[] = {1,0,2,3,4,5,6,7,8,9};
        return build_number(digs, sizeof(digs));
    }

    /* n > 10 : the requirement reduces to digits 0..9 once each.
       The minimal multiple of 3 satisfying that is the same as for n = 10. */
    return 1023456789;
}

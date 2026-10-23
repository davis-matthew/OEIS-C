/*
 * A037889  (1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*3^i} is base 3
 *          representation of n and e(i) are digits d(i) in reverse order.
 * https://oeis.org/A037889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * ∑ |d(i) – d(k-1–i)|, where d(i) are the base‑3
 *            digits of n (least significant first)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses no external utilities and works for any build
   where seq_t and idx_t are defined. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for the offset 1 */

    /* Base‑3 digits of n (least significant first).  An unsigned
       64‑bit integer needs at most 41 base‑3 digits, so 64 is safe. */
    uint8_t digits[64];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 3);
        t /= 3;
    }

    /* Sum of absolute differences between digits and their reverse. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d1 = digits[i];
        uint8_t d2 = digits[len - 1 - i];
        uint8_t diff = d1 > d2 ? d1 - d2 : d2 - d1;
        sum += (seq_t)diff;
    }

    return sum / 2;   /* guaranteed to be an integer */
}

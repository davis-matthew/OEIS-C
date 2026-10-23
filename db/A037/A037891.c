/*
 * A037891  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*5^i} is the base 5
 *          representation of n and e(i) are the digits d(i) in reverse order.
 * https://oeis.org/A037891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * sum_{i} |d(i)-e(i)|, where d(i) are the base‑5
 *            digits of n (least significant first) and e(i) are those digits
 *            in reverse order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 explicitly (offset is 1, but this keeps the
       function total‑safe). */
    if (n == 0)
        return (seq_t)0;

    /* Store the base‑5 digits of n.  64 slots are more than enough
       for any 64‑bit value. */
    unsigned char digits[64];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 5);
        tmp /= 5;
    }

    /* Sum the absolute differences between each digit and its
       counterpart in the reversed list. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char d = digits[i];
        unsigned char e = digits[len - 1 - i];
        unsigned char diff = (d >= e) ? (d - e) : (e - d);
        sum += (seq_t)diff;
    }

    /* Final value divides the sum by two. */
    return sum / 2;
}

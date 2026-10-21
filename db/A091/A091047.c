/*
 * A091047  a(n) = the final value of n reached through repeated
 *          interpretation of n as a base b+1 number where b is the largest
 *          digit of n.
 * https://oeis.org/A091047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = final value of n after repeatedly interpreting its
 *            decimal digits in base (max_digit(n)+1) until the value no
 *            longer changes
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t next_value(seq_t value)
{
    int digits[50];
    int len = 0;
    seq_t tmp = value;
    int max_d = 0;

    /* extract digits and find the largest digit */
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        if (d > max_d) max_d = d;
        digits[len++] = d;
        tmp /= (seq_t)10;
    }
    if (len == 0) {                 /* value == 0 (not expected) */
        digits[0] = 0;
        len = 1;
    }

    /* If the maximum digit is 9 or the number is a single digit,
       the interpretation never changes, so return the same value. */
    if (max_d == 9 || len == 1) return value;

    seq_t base = (seq_t)(max_d + 1);
    seq_t result = 0;

    /* evaluate the digits in the new base (most significant first) */
    for (int i = len - 1; i >= 0; --i) {
        result = result * base + (seq_t)digits[i];
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t value = (seq_t)n;
    seq_t next;

    while (true) {
        next = next_value(value);
        if (next == value) break;    /* no change -> fixed point */
        value = next;
    }
    return value;
}

/*
 * A091048  a(n) = the number of steps needed to reach the final value of n
 *          via repeated interpretation of n as a base b+1 number where b is
 *          the largest digit of n.
 * https://oeis.org/A091048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 653, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A091048:
 * a(1) is the first term of the b‑file.
 * a(n) is the number of steps needed to reach the final value of n by
 * repeatedly interpreting the current value as a base (b+1) number,
 * where b is the largest digit of that value.
 * A value stops when it has a digit 9 or it is a single‑digit number.
 * For example: 18 → (in base 9) 17 → (in base 8) 15 → (in base 6) 11
 * → (in base 2) 3 → 3 (final), so a(18) = 4.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = n;          /* current value */
    seq_t steps = 0;      /* number of steps performed */

    /* temporary buffer for digits (max length assumed 64) */
    int digits[64];
    int len;

    for (;;) {
        /* Stop if the number is a single digit (final value) */
        if (m < 10) {
            return steps;
        }

        /* Determine the largest digit and store all digits in reverse order */
        seq_t tmp = m;
        int max_d = 0;
        len = 0;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            digits[len++] = d;
            if (d > max_d) {
                max_d = d;
            }
            tmp /= 10;
        }

        /* If the largest digit is 9, the value cannot be reduced further */
        if (max_d == 9) {
            return steps;
        }

        /* Compute the next value by interpreting the digits in base (max_d + 1) */
        seq_t base = (seq_t)max_d + 1;
        seq_t next = 0;
        seq_t weight = 1;
        for (int i = 0; i < len; i++) {
            next += (seq_t)digits[i] * weight;
            weight *= base;
        }

        m = next;
        steps++;
    }
}

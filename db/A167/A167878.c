/*
 * A167878  A167877(n) + n.
 * https://oeis.org/A167878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest integer ≤ 2*n whose base‑3 representation *
 *            contains no digit 1. Equivalently, take 2*n, replace the first
 *            digit 1 * (from the most significant side) by 0 and set all
 *            lower digits to 2
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* 2 * n */
    seq_t limit = (seq_t)n * 2;

    /* find the highest power of 3 ≤ limit */
    seq_t power = 1;
    while (power <= limit) {
        power *= 3;
    }
    power /= 3;                /* now power is the most significant place */

    seq_t result = 0;
    bool seen_one = false;
    seq_t rem = limit;         /* remaining part of limit to process */

    while (power > 0) {
        seq_t digit = rem / power;
        rem -= digit * power;

        if (!seen_one) {
            if (digit == 1) {       /* first 1 encountered */
                seen_one = true;
                /* this digit is 0, so nothing is added for it */
                power /= 3;
                /* fill all remaining digits with the maximal allowed value 2 */
                while (power > 0) {
                    result += 2 * power;
                    power /= 3;
                }
                return result;
            } else if (digit == 2) { /* digit 2 is allowed */
                result += 2 * power;
            }
            /* digit 0 contributes nothing */
        } else { /* already encountered a 1 earlier, so all remaining digits are 2 */
            result += 2 * power;
        }

        power /= 3;
    }

    return result;   /* handles the case where limit itself has only digits 0 or 2 */
}

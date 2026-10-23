/*
 * A182772  Beatty sequence for (5-sqrt(3))/2.
 * https://oeis.org/A182772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(5-sqrt(3))/2)
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;       /* current value a(n) */
    int64_t p = 0;          /* numerator of fractional part: (p + q*sqrt(3))/2 */
    int64_t q = 0;          /* coefficient of sqrt(3) in the fractional part */

    /* Iterate from 1 to n, updating the fractional part and the sequence value */
    for (idx_t i = 1; i <= n; ++i) {
        /* Add the increment delta = (3 - sqrt(3))/2 to the fractional part:
           p += 3, q -= 1 (since (p + q*sqrt(3))/2 + (3 - sqrt(3))/2 = (p+3 + (q-1)*sqrt(3))/2 ) */
        p += 3;
        q -= 1;

        /* Determine whether the fractional part reaches or exceeds 1:
           Test if (p + q*sqrt(3))/2 >= 1, equivalently p + q*sqrt(3) >= 2 */
        int64_t a_val = p - 2;   /* part of the inequality: a_val + q*sqrt(3) >= 0 */
        int64_t b_val = q;       /* coefficient of sqrt(3) */

        bool ge_one = false;     /* true when the fractional part >= 1 */

        if (b_val > 0) {
            /* a_val + b_val*sqrt(3) >= 0
               If a_val >= 0, it holds automatically.
               Otherwise, 0 < -a_val, so we compare squares: a_val^2 <= 3*b_val^2 */
            if (a_val >= 0) {
                ge_one = true;
            } else {
                int64_t a2 = a_val * a_val;
                int64_t b2 = b_val * b_val;
                ge_one = (a2 <= 3 * b2);
            }
        } else if (b_val < 0) {
            /* a_val + b_val*sqrt(3) >= 0
               If a_val <= 0, the inequality cannot hold.
               Otherwise, a_val > 0, and we compare squares: a_val^2 >= 3*(-b_val)^2 */
            if (a_val <= 0) {
                ge_one = false;
            } else {
                int64_t a2 = a_val * a_val;
                int64_t b2 = b_val * b_val;   /* b2 is positive */
                ge_one = (a2 >= 3 * b2);
            }
        } else { /* b_val == 0 */
            ge_one = (a_val >= 0);
        }

        if (ge_one) {
            /* The fractional part reaches or exceeds 1:
               Increment the sequence by 2 and wrap the fractional part by subtracting 1.
               Subtracting 1 from the fractional part corresponds to reducing p by 2. */
            result += (seq_t)2;
            p -= 2;
        } else {
            /* Increment by 1 only */
            result += (seq_t)1;
        }
    }

    return result;
}

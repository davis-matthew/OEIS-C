/*
 * A044931  a(n) = so-se, where so(se)=sum of odd(even) base 9 run lengths of
 *          n.
 * https://oeis.org/A044931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum over runs of equal digits in the base‑9
 *            representation of n of (len(run) if len is odd, otherwise
 *            –len(run))
 * Range:     i8 n <= 8532, i16 n <= 8532, i32 n <= 8532, i64 n <= 8532,
 *            i128 n <= 8532, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* n=0 not in the supported range, return 0 for safety */
        return 0;
    }

    /* Collect base‑9 digits of n (least significant first) */
    unsigned char digits[64];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = t % 9;
        t /= 9;
    }

    /* Run‑length analysis from the most significant to the least */
    seq_t odd_sum = 0;
    seq_t even_sum = 0;
    size_t i = len - 1;
    unsigned char cur = digits[i];
    size_t run = 1;

    while (i > 0) {
        --i;
        unsigned char d = digits[i];
        if (d == cur) {
            ++run;
        } else {
            if (run & 1)
                odd_sum += (seq_t)run;
            else
                even_sum += (seq_t)run;
            cur = d;
            run = 1;
        }
    }

    /* Final run */
    if (run & 1)
        odd_sum += (seq_t)run;
    else
        even_sum += (seq_t)run;

    return odd_sum - even_sum;
}

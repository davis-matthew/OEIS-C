/*
 * A095565  a(n) = 86 written in base 14 - n.
 * https://oeis.org/A095565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the base-(14-n) representation of 86.
 *            For n ≤ 13 this is computed explicitly. For n > 13 the base
 *            would be <1; the sequence is conventionally defined to remain
 *            constant at the base‑1 representation (86 ones)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define DIGIT_MAX 10  /* 86 in binary has at most 7 digits; 10 is safe */

seq_t a(idx_t n)
{
    /* Base for representation of 86 */
    if (n <= 13)
    {
        int base = 14 - (int)n;

        /* base ≥ 2 : normal conversion */
        if (base >= 2)
        {
            int temp = 86;
            int digits[DIGIT_MAX];
            int cnt = 0;

            /* extract digits in reverse order */
            while (temp > 0)
            {
                digits[cnt++] = temp % base;
                temp /= base;
            }

            /* build decimal value from most significant digit to least */
            seq_t res = 0;
            for (int i = cnt - 1; i >= 0; --i)
            {
                res = res * 10 + digits[i];
            }
            return res;
        }

        /* base == 1 : unary representation -> 86 ones */
        if (base == 1)
        {
            seq_t res = 0;
            for (int i = 0; i < 86; ++i)
                res = res * 10 + 1;   /* 1 digit appended */
            return res;
        }
    }

    /* For n > 13 (base < 1) the value stabilises to the
       86‑digit unary string. */
    seq_t res = 0;
    for (int i = 0; i < 86; ++i)
        res = res * 10 + 1;
    return res;
}
